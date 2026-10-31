// Main File that is dependent on the log4me library without including the compilation of the log4me.cpp file.
#include <macros>
#include "log4me.h"
#include <iostream>
#include <bora>

struct TestOnWasmSDK : public bora::ISDKCommand
{
    TestCommandData data;
    
        TestOnWasmSDK(const char* text, int value)
    {
        data.value = value;
        data.text = text;
        // data.ints[0] = value;
        // data.ints[1] = value+1;
    }

    static constexpr const char* RuntimeName() { return "TestOnWasm"; }
    static constexpr const char* RuntimeCategory() { return CommandCategories::NativeTesting; }
};


// class TestCommand : public bora::ICommandBase {
// public:
//     const char* GetCategory() const override { return CommandCategories::NativeTesting; }
//     const char* GetName() const override { return "TestOnWasm"; }

//     TestCommandData* data;

//     void Execute(void* payload) override {

//         data = reinterpret_cast<TestCommandData*>(payload);
//         printf("%s\n", data->text);
//         throw std::exception();
//     }
// };


class TestCommandList : public bora::ICommandList<MutexVector> {
public:
    TestCommandList() : ICommandList(CommandCategories::NativeTesting, "TestList") {
    }

    void TestTheCommand(TestOnWasmSDK& value){
        mBuffer.Push(value);
    }

    void JustCommit(){
        Commit();
    }
};


// BORA_REGISTER_COMMAND(TestCommand);

#include <atomic>

// int main() {
//     // 1. Allocate shared variables on the shared heap
//     std::atomic<int>* shared_i = new std::atomic<int>(0);
//     std::atomic<bool>* stop_signal = new std::atomic<bool>(false);

//     printf("Main regular printfs2 - nothing special\n");
//     printf("shared_i main address: %p (value: %d)\n", (void*)shared_i, shared_i->load());
//     printf("stop_signal main address: %p (value: %d)\n", (void*)stop_signal, stop_signal->load());

//     std::cin.get();

//     // 2. Spawn Thread 1 (Reader)
//     auto thread = bora::thread([](std::atomic<int>* shared_i, std::atomic<bool>* stop_signal) {
//         printf("stop_signal thread address: %p \n", (void*)stop_signal);
//         printf("shared_i thread address: %p \n", (void*)shared_i);

//         int last_val = -1;
        
//         // Loop until stop signal becomes true
//         while (stop_signal->load(std::memory_order_acquire) == false) {
//             int val = shared_i->load(std::memory_order_acquire);
//             if (val != last_val) {
//                 log_print("test");
//                 printf("thread saw i = %d\n", val);
//                 last_val = val;
//             }
//         }
//         printf("Reader thread shutting down cleanly.\n");
//     }, shared_i, stop_signal);  


//     auto thread2 = bora::thread([](std::atomic<int>* shared_i, std::atomic<bool>* stop_signal) {
//         printf("stop_signal thread address: %p \n", (void*)stop_signal);
//         printf("shared_i thread address: %p \n", (void*)shared_i);

//         // Loop until stop signal becomes true
//         while (stop_signal->load(std::memory_order_acquire) == false) {
//             shared_i->fetch_add(1, std::memory_order_acquire);
//         }
//         printf("Incrementer thread shutting down cleanly.\n");
//     }, shared_i, stop_signal);

//     thread.setName("Reader");
//     thread2.setName("Incrementer");


//     // 4. Wait for user input to trigger shutdown
//     std::cout << "Press Enter to stop threads...\n";
//     std::cin.get();
//     thread.interrupt();
//     printf("Thread 1 Name: %s\n", thread.getName());
//     printf("Thread 2 Name: %s\n", thread2.getName());
//     std::cin.get();
//     thread.resume();
//     std::cin.get();
//     // 5. Broadcast the stop signal
//     stop_signal->store(true, std::memory_order_relaxed);
//     thread.join();
//     // thread2.join();
//     // thread2.join();

//     // (Optional: if you have a .join() method implemented yet, call it here 
//     // to ensure the threads finish before you delete the memory)

//     // 6. Clean up shared heap resources
//     delete shared_i;
//     delete stop_signal;
    
//     printf("Main exited successfully.\n");
   
//     return 0;
// }

#include <iostream>
#include <fstream>
#include "stl/file.h"

int main() {
    // open file hello.txt and ../hello.txt through standard c++
    // in this test, hello.txt should be readable but ../hello_txt is unreadable without permission.
    std::cout << "[Guest] Attempting to open 'hello.txt'...\n";
    bora::File file1("./hello.txt");
    
    if (file1.is_open()) {
    // 1. Jump to the end of the file to get its size
    file1.seekg(0, std::ios::end);
    std::streamsize fileSize = file1.tellg();
    
    // 2. Jump back to the beginning to read the content
    file1.seekg(0, std::ios::beg);

    // 3. Pre-allocate the string buffer for maximum efficiency (Zero re-allocations!)
    std::vector<char> sharedBuffer(fileSize);
    if (fileSize > 0) {
        file1.read(sharedBuffer.data(), fileSize);
    }
    std::string content(sharedBuffer.begin(), sharedBuffer.end());
    
    std::cout << "[Guest] SUCCESS: Read 'hello.txt' (" << fileSize << " bytes) -> Content: " << content << "\n";
    } else {
        std::cout << "[Guest] FAILED: Could not open 'hello.txt'.\n";
    }

    // Test 2: Attempt a path traversal attack to access a restricted file outside root
    std::cout << "\n[Guest] Attempting to open '../hello.txt' (Traversal Test)...\n";
    bora::File file2("../hello.txt");
    
    if (file2.is_open()) {
        std::cout << "[Guest] SUCCESS: Read '../hello.txt' (Security Warning: Should have been blocked!)\n";
    } else {
        std::cout << "[Guest] BLOCKED: Supervisor correctly denied access to '../hello.txt'.\n";
    }
    return 0;

    // 1. Allocate shared variables on the shared heap
    std::atomic<bool> stop_signal = true;
    {
    size_t alloc_size = 4ULL * 1024 * 1024 * 1024;
    void* ptr = malloc(alloc_size);
    if (ptr != nullptr) {
        // Fill the 4MB block with 'A'
        memset(ptr, 'A', alloc_size);
        free(ptr);
    } else {
        printf("Allocation failed!\n");
    }
    std::cin.get();
    }

    // auto thread = bora::thread([]() {
    //   while (true){
    //     void* offset = malloc(32);
    //     printf("Thread Pointer address/offset: %p\n", static_cast<void*>(offset));
    //     free(offset);
    //   }
    // });  
    // while(true){
    //     void* offset = malloc(32);
    //     printf("Main Pointer address/offset: %p\n", static_cast<void*>(offset));
    //     free(offset);
    // }

    // std::vector<int> shared_vector;
    // bora::Mutex vector_mutex;

    // auto thread = bora::thread([](std::vector<int>* vec, bora::Mutex* mtx) {
    // while (true) {
    //     std::lock_guard<bora::Mutex> lock(*mtx);
    //     vec->push_back(42);
    //     printf("%zu\n", vec->size());
    // }
    // }, &shared_vector, &vector_mutex);

    // while(true){
    //     std::lock_guard<bora::Mutex> lock(vector_mutex);
    //     shared_vector.push_back(21);
    //     printf("%zu\n", shared_vector.size());
    // }

    auto list = TestCommandList();
    
    const int kNumThreads = 12;
std::vector<bora::thread> threads;
threads.reserve(kNumThreads);

for (int i = 0; i < kNumThreads; ++i) {
    // Unique thread identifier number (1-based index)
    int thread_id = i + 1;

    // Emplace and spawn the thread
    threads.emplace_back([](TestCommandList* list, std::atomic<bool>* stop_signal, int id) {
        // Optional startup wait loop (matching your original logic)
        while (stop_signal->load()) {
            // Wait or yield if needed
        }

        int counter = 0;
        // Dynamically format the thread label e.g., "thread 1", "thread 2", etc.
        std::string thread_label = "thread " + std::to_string(id);

        while (!stop_signal->load())
        {
            counter++;
            auto command = TestOnWasmSDK(thread_label.c_str(), counter);
            list->TestTheCommand(command);
            log_print("test 2");
        }
    }, &list, &stop_signal, thread_id);

    // Set the thread name dynamically e.g., "Commandlist 1", "Commandlist 2", etc.
    std::string thread_name = "Commandlist " + std::to_string(thread_id);
    threads[i].setName(thread_name.c_str());
}

    stop_signal = false;
    std::cout << "Press Enter to Commit threads...\n";
    std::cin.get();
    stop_signal = true;
    list.JustCommit();
    printf("Main exited successfully.\n");  
    return 0;
}