#include "../include/bora.h"
#include <string>

using namespace bora::stl;

struct MyStruct {
    int x;
    MyStruct(int val) : x(val) {}
};


bora::stl::shared_ptr<MyStruct> make_mystruct(int value) {
    void* raw_mem = reinterpret_cast<void*>(bora::memory::allocate(sizeof(MyStruct)));
    MyStruct* obj = new (raw_mem) MyStruct(value); // placement new
    return bora::stl::shared_ptr<MyStruct>(obj);
}

// good progress so far
int main() {
  char buffer[64];
int written = bora::input::cin((int)buffer, sizeof(buffer));  // from this input
if (written > 0) {
        buffer[written] = '\0';
}



bora::logging::print(BORA_VERSION);

bora::logging::messageBoxConfig config;
config.title = "Bora Window";
config.message = "Hello, Bora! This is a message box.";


bora::stl::thread([](int arg) {
    // This is a thread function
    // You can use the arg parameter to pass data to the thread
    // For example, you can print the argument
bora::stl::string title("Hello World! This is a string in a thread. ");

for (int i = 0; i < 999; ++i) { // 100 * 2 MBS = 200 MB for string system
    title.append("Bora is awesome! ");
    if (i % 1000 == 0) {
        bora::logging::printf("Thread iteration: %d\n", i);
    }
    

bora::logging::print(title.c_str()); // Should print 'B'
}


}, 42);  // Start a thread with an argument


bora::logging::print("Hello, Bora!");

bora::logging::messageBox(config);

// creates a window

bora::window::create_window(buffer, 1920, 1080);

    const char* my_text = "Hello from WASM!";
    uint64_t len = strlen(my_text) + 1;  // Include null terminator

    // Step 1: Allocate space to receive the allocated pointer from host
    uint64_t* result_ptr_location = (uint64_t*)malloc(sizeof(uint64_t));
    if (!result_ptr_location) {
        bora::logging::printError("malloc failed\n");
        return 1;
    }

    // Step 2: Call host allocator, pass pointer offset to receive result
    uint64_t result = bora::memory::allocateWithPointer(len, (uint64_t)(uintptr_t)result_ptr_location);
    if (result != 0) {
        bora::logging::printError("Host allocation failed");
        free(result_ptr_location);
        return 1;
    }

    // Step 3: Read the allocated memory offset
    uint64_t str_offset = *result_ptr_location;
    free(result_ptr_location); // We're done with this

    // Step 4: Treat it as a char* and copy the string into it
    char* str_memory = (char*)(uintptr_t)str_offset;
    strcpy(str_memory, my_text); // Write to allocated WASM memory

    // Step 5: Use it as a const char*
    const char* final_str = str_memory;
    bora::logging::printf("Stored string: %s\n", final_str);


  auto my_struct_ptr = make_mystruct(42);
    
    bora::logging::printf("MyStruct x value: %d\n", my_struct_ptr->x);
    my_struct_ptr->x = 100; // Modify the value
    bora::logging::printf("Modified MyStruct x value: %d\n", my_struct_ptr->x);
    
   shared_ptr<MyStruct> test2 = move(my_struct_ptr);
   bora::logging::printf("Test2 MyStruct x value: %d\n", test2->x);
   bora::logging::printf("MyStruct x value after move: %d\n", my_struct_ptr->x); // Should be 0 or undefined since moved
    bora::stl::atomic::create(1);
    return 0;
}