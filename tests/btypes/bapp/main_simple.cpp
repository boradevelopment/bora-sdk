#include <bora>
#include <symbols/common/TestCommand.h>
#include <iostream>

using namespace bora;

// This is the function the thread will run
void StressWorker(long iterations) {
    // We create a local CommandList for this thread to avoid mutex locking
    // which would slow down the test and measure the lock instead of the system.
    TestCommandList* list = new TestCommandList();
    for (long i = 0; i < iterations; ++i) {
        //  bora::logging::print("Allocating");
        std::cout << "Allocating - " << i << std::endl;
        auto cmd = TestCommandSDK(i);
        list->JustPush(cmd);
    }
    list->JustCommit();
    list->Clear();
    std::cout << "Now? Memory 4GB Test!";
    std::cin.get();
    delete list;
}

// In your main Wasm start or entry point:
void RunStressTest() {
    const int numThreads = 4;
    const long cmdsPerThread = 100000; // 100k commands per thread
    
    for (int i = 0; i < numThreads; ++i) {
        // Create the thread and pass the iteration count as the argument
        bora::stl::thread t(StressWorker, cmdsPerThread);
        t.join();
    }
}

static void updateFunctionWindow(const char* wndID, void* userObject, unsigned int msg, u64 wParam, intptr_t lParam){
    printf(wndID);
}

int main(){
StressWorker(1000);
    void* test = (void*)bora::memory::allocate(4ULL * 1024 * 1024 * 1024);
    if (!test) {
        std::cout << "4GB Unavailable!";
        // allocation failed
        return 1;
    }
    std::cin.get();
    const u64 size = (4ULL * 1024 * 1024 * 1024)-1;
    std::memset(test, 'A', size);
    // Print first 500 characters
    char* data = static_cast<char*>(test);
    for (size_t i = 0; i < 500; i++) {
        std::cout << data[i];
    }
    std::cin.get();
std::cout << "\n";
bora::memory::deallocate((u64)test);
std::cout << "4GB test done";

return 0;

// set all characters to A and print 500 of them
// release and return 0;
WindowConfig config;
config.id = "randomthingy";
config.update = updateFunctionWindow;
bnUserWindow e(config);
TextureDesc desc;
desc.height = 1920;
desc.width = 1080;
auto resource = e.getGraphics()->CreateTexture(desc, nullptr);
e.run();

return 0;


// std::cin.get();
//   int *array = new int[100];
//   delete [] array;
//   return array[0];

}