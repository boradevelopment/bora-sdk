#include <bora>
#include <symbols/common/TestCommand.h>
#include <iostream>
#include <chrono> // Added for high-precision timing
#include <cassert>
#include <malloc.h> 

using namespace bora;

// This is the function the thread will run
void StressWorker(long iterations) {
    // We create a local CommandList for this thread to avoid mutex locking
    // which would slow down the test and measure the lock instead of the system.
    TestCommandList* list = new TestCommandList();
    for (long i = 0; i < iterations; ++i) {
        auto cmd = TestCommandSDK(i);
        list->JustPush(cmd);
    }
    list->JustCommit();
    list->Clear();
    delete list;
}

// In your main Wasm start or entry point:
void RunStressTest() {
    const int numThreads = 4;
    const long cmdsPerThread = 100000; // 100k commands per thread
    
    std::cout << "=== STARTING THREADED STRESS TEST (" << (numThreads * cmdsPerThread) << " total commands) ===" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        // Create the thread and pass the iteration count as the argument
        bora::stl::thread t(StressWorker, cmdsPerThread);
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "=== THREADED STRESS TEST COMPLETE ===" << std::endl;
    std::cout << "Time taken: " << elapsed.count() << " ms (" << (elapsed.count() / 1000.0) << " seconds)" << std::endl;
}

static void updateFunctionWindow(const char* wndID, void* userObject, unsigned int msg, u64 wParam, intptr_t lParam){
    printf("%s\n", wndID);
}


struct FragmentationStatus {
    bool is_fragmented;
    size_t total_free_bytes;
    size_t free_chunk_count;
    size_t max_allocatable_block;
    double fragmentation_ratio; // 0.0 = clean, 1.0 = severely fragmented
};

FragmentationStatus check_bora_fragmentation() {
   struct mallinfo info = mallinfo();
    
    // mallinfo fields use 32-bit integers
    size_t free_bytes  = static_cast<size_t>(info.fordblks); // Total space in free chunks
    size_t free_chunks = static_cast<size_t>(info.ordblks);  // Number of disjoint free blocks

    if (free_bytes == 0) {
        return { false, 0, 0, 0, 0.0 };
    }

    // Attempt to allocate total reported free space in a single contiguous block
    void* large_ptr = (void*)bora::memory::allocate(free_bytes);
    size_t max_block = 0;

    if (large_ptr != nullptr) {
        max_block = free_bytes;
        bora::memory::deallocate((u64)large_ptr); // Clean up test allocation
    } else {
        // Allocation failed: space is split across multiple chunks
        max_block = free_bytes / (free_chunks > 0 ? free_chunks : 1);
    }

    bool fragmented = (free_chunks > 1) || (large_ptr == nullptr);
    double ratio = 1.0 - (static_cast<double>(max_block) / free_bytes);

    return {
        fragmented,
        free_bytes,
        free_chunks,
        max_block,
        ratio
    };
}


int main(){
{
    std::cout << "[RUNNING] Test: Expecting NO Fragmentation...\n";

    constexpr size_t BLOCK_SIZE = 1024; // 1 KB
    constexpr int NUM_BLOCKS = 100;
    void* ptrs[NUM_BLOCKS];

    // 1. Allocate memory blocks
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        ptrs[i] = (void*)bora::memory::allocate(BLOCK_SIZE);
        assert(ptrs[i] != nullptr && "Allocation failed");
    }

    // 2. Deallocate ALL blocks sequentially
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        bora::memory::deallocate((u64)ptrs[i]);
    }

    // 3. Assert NO fragmentation exists
    FragmentationStatus status = check_bora_fragmentation();

    std::cout << "Free Chunks: " << status.free_chunk_count << "\n";
    std::cout << "Fragmentation Ratio: " << (status.fragmentation_ratio * 100.0) << "%\n";

    // Standard dlmalloc coalesces freed adjacent memory into 1 single chunk
    assert(!status.is_fragmented && "Memory should NOT be fragmented after full deallocation!");
    assert(status.free_chunk_count <= 1 && "Expected at most 1 contiguous free block");
    std::cout << "[PASSED] Memory is clean and unfragmented!\n\n";
}
{
    std::cout << "[RUNNING] Test: Expecting DETECTED Fragmentation...\n";

    constexpr size_t BLOCK_SIZE = 4096; // 4 KB
    constexpr int NUM_BLOCKS = 50;
    void* ptrs[NUM_BLOCKS];

    // 1. Allocate blocks continuously
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        ptrs[i] = (void*)malloc(BLOCK_SIZE);
        assert(ptrs[i] != nullptr);
    }

    // 2. Deallocate ONLY EVEN-INDEXED blocks (0, 2, 4, 6...)
    // This creates "checkerboard" holes in the heap
    for (int i = 0; i < NUM_BLOCKS; i += 2) {
        free(ptrs[i]);
        ptrs[i] = nullptr;
    }

    // 3. Assert fragmentation IS detected
    FragmentationStatus status = check_bora_fragmentation();

    std::cout << "Total Free Space: " << status.total_free_bytes << " bytes\n";
    std::cout << "Disjoint Free Chunks: " << status.free_chunk_count << "\n";
    std::cout << "Fragmentation Ratio: " << (status.fragmentation_ratio * 100.0) << "%\n";

    assert(status.is_fragmented && "Fragmentation should have been detected!");
    assert(status.free_chunk_count > 1 && "Memory should have multiple disjoint free chunks");

    std::cout << "[PASSED] Fragmentation successfully detected!\n\n";

    // Cleanup remaining odd-indexed blocks
    for (int i = 1; i < NUM_BLOCKS; i += 2) {
        free(ptrs[i]);
    }
}

    // std::cout << "=== STARTING SINGLE-THREADED WORKER ===" << std::endl;
    // // 1. Capture start timestamp
    // auto start = std::chrono::high_resolution_clock::now();
    // // Run the allocations
    // StressWorker(1);
    // // 2. Capture end timestamp and calculate difference
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double, std::milli> elapsed = end - start;

    // std::cout << "=== SINGLE-THREADED WORKER COMPLETE ===" << std::endl;
    // std::cout << "Time taken for 1 iterations: " << elapsed.count() << " ms (" << (elapsed.count() / 1000.0) << " seconds)\n\n";

    // Initialize UI Windowing/Graphics Context
    // WindowConfig config;
    // config.id = "randomthingy";
    // config.update = updateFunctionWindow;
    // bnUserWindow e(config);
    
    // TextureDesc desc;
    // desc.height = 1920;
    // desc.width = 1080;
    // auto resource = e.getGraphics()->CreateTexture(desc, nullptr);
    // e.run();

    return 0;
}