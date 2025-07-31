#define BORA_USE_NAMESPACE
#define BORA_STL_USE_NAMESPACE
#define B_HideStringWarnings
#include <bora>

struct MyStruct {
        int x;
        u8* buffer;

        MyStruct(int val) : x(val), buffer((u8*)memory::allocate(8 * 1024 * 1024)) {}
    };


    shared_ptr<MyStruct> make_mystruct(int value) {
        MyStruct* obj = new MyStruct(value);
        return shared_ptr<MyStruct>(obj);
    }

    // good progress so far
    int main() {
    logging::printf("bora sdk version: %s", bora::versioning::sdk::get_bora_sdk_version());
    vector<int> test;
    test.push_back(5);

    logging::printf("first data: %d", test.data());

    logging::print("Hello, Bora!");
    char buffer[64];
    int written = input::cin((long)buffer, sizeof(buffer));  // from this input
    if (written > 0) {
            buffer[written] = '\0';
    }

    logging::messageBox({
        string("Your input:").c_str(), buffer, logging::MessageBoxType::PROBLEM
    });


    logging::print("Before allocation");
    auto testData = shared_ptr<MyStruct>(new MyStruct(1));
    input::cin((long)buffer, sizeof(buffer)); 
    logging::print("After allocation");

    logging::print("Before deallocation");
    testData.~shared_ptr();
    input::cin((long)buffer, sizeof(buffer)); 
    logging::print("After deallocation");

    auto my_struct_ptr = make_mystruct(42);
    input::cin((long)buffer, sizeof(buffer)); 

    logging::printf("MyStruct x value: %d\n", my_struct_ptr->x);
    my_struct_ptr->x = 100; // Modify the value
    logging::printf("Modified MyStruct x value: %d\n", my_struct_ptr->x);
    input::cin((long)buffer, sizeof(buffer));     
        
    auto test2 = move(my_struct_ptr);
    logging::printf("Test2 MyStruct x value: %d\n", test2->x);
    logging::printf("MyStruct x value after move: %d\n", my_struct_ptr->x); // Should be 0 or undefined since moved
    
    if(my_struct_ptr.get() == nullptr) {
        logging::print("my_struct_ptr is now null after move.\n");
    }
    

    logging::print(bora::versioning::sdk::modeToString());

    input::cin((long)buffer, sizeof(buffer)); 

    return 0;

    // logging::messageBoxConfig config;
    // config.title = "Bora Window";
    // config.message = "Hello, Bora! This is a message box.";

    // thread([](int arg) {
    //     // This is a thread function
    //     // You can use the arg parameter to pass data to the thread
    //     // For example, you can print the argument
    // string title("Hello World! This is a string in a thread. ");

    // for (int i = 0; i < 999; ++i) { // 100 * 2 MBS = 200 MB for string system
    //     title.append("Bora is awesome! ");
    //     if (i % 1000 == 0) {
    //         logging::printf("Thread iteration: %d\n", i);
    //     }
        

    // logging::print(title.c_str()); // Should print 'B'
    // }


    // }, 42);  // Start a thread with an argument


  

    // logging::messageBox(config);

    // // creates a window

    // window::createWindow(buffer, 1920, 1080);

    //     const char* my_text = "Hello from WASM!";
    //     u64 len = string::strlen(my_text) + 1;  // Include null terminator

    //     u64* result_ptr_location = (u64*)memory::allocate(sizeof(u64));
    //     if (!result_ptr_location) {
    //         logging::printError("alloc failed\n");
    //         return 1;
    //     }

    //     u64 result = memory::allocateWithPointer(len, (u64)(u64)result_ptr_location);
    //     if (result != 0) {
    //         logging::printError("Host allocation failed");
    //         memory::deallocate((u64)result_ptr_location);
    //         return 1;
    //     }
        
    //     u64 str_offset = *result_ptr_location;
    //     memory::deallocate((u64)result_ptr_location); // We're done with this

    //     char* str_memory = (char*)(u64)str_offset;
    //     string::copy(str_memory, my_text); // Write to allocated WASM memory

    //     const char* final_str = str_memory;
    //     logging::printf("Stored string: %s\n", final_str);




        return 0;
    }
