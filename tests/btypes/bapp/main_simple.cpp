#include <bora>
#include <symbols/common/TestCommand.h>

using namespace bora;

// This is the function the thread will run
void StressWorker(long iterations) {
    // We create a local CommandList for this thread to avoid mutex locking
    // which would slow down the test and measure the lock instead of the system.
    TestCommandList* list = new TestCommandList();

    for (long i = 0; i < iterations; ++i) {
        bora::logging::print("Allocating");
        int* heapPtr = new int(i);
        list->JustPush(*heapPtr);
        delete heapPtr;
      
    }
    
    // Final flush
    bora::logging::print("Commiting");
    list->JustCommit();
    list->Clear();
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

int main(){

StressWorker(500);

}