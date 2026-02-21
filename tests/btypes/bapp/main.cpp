#define BORA_USE_NAMESPACE
#define BORA_STL_USE_NAMESPACE
#define B_HideStringWarnings
#include <bora>

using namespace bora::logging;

static void updateFunction(bnWindow* window) {

}

struct MyStruct {
        int x;
        u8* buffer;

        MyStruct(int val) : x(val), buffer((u8*)memory::allocate(8 * 1024 * 1024)) {
            memory::set(buffer, 'W', 8 * 1024 * 1024);
        }
        ~MyStruct(){
            logging::printf("%llu", (u64)buffer);
            memory::deallocate((u64)buffer);
        }
    };


    shared_ptr<MyStruct> make_mystruct(int value) {
        MyStruct* obj = new MyStruct(value);
        return shared_ptr<MyStruct>(obj);
    }


    // good progress so far
    int main() {

    char buffer[64];
    int written = input::cin((long)buffer, sizeof(buffer));  // from this input
    if (written > 0) {
            buffer[ written] = '\0';
    }

    
size_t size = 1ULL * 1024 * 1024 * 1024; // 1 GB

char* buffer2 = (char*)memory::allocate(size);
if (!buffer2) {
    logging::printError("You do not have enough memory");
    return 1;
}

//memset(buffer2, 'A', size - 1);
memory::set(buffer2, 'A', size-1);
buffer2[size - 1] = '\0';  // Null-terminate if treating as a string

for (size_t i = 0; i < 500; ++i) {
      auto character = buffer2[i];
      logging::printf("\a[%zu] = %c\n", i, character);
}

memory::deallocate((u64)buffer2);
    
   
    logging::printf("bora sdk version: %s", bora::versioning::sdk::get_bora_sdk_version());
    vector<int> test;
    test.push_back(5); // index = 0

    test[0] = 4; // 5 will be overwritten to 4

    printf("first data: %d", test.data()[0]);

    print("Hello, Bora!");

    messageBox({
        "Your input:", buffer, MessageBoxType::PROBLEM
    });

    logging::print(bora::versioning::sdk::modeToString());
    auto config = WindowConfig();
    config.title = "AcidTrueSegundaNih";
    config.update = updateFunction;
    auto window = bnUserWindow(config);
    window.run();
    auto graphics = window.getGraphics();
    graphics->CreateTextureCommand({});
    return 0;

    }
