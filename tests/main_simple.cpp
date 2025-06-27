#include <bora>

using namespace bora;

int main(){
auto e = stl::thread([](int arg) {
    // This is a thread function
    // You can use the arg parameter to pass data to the thread
    // For example, you can print the argument
stl::string title("Hello World! This is a string in a thread.");

for (int i = 0; i < 999; ++i) {
    title.append("Bora is awesome! ");
bora::logging::print(title.c_str()); 
}


}, 1);  // Start a thread with an argument

bora::logging::messageBoxConfig config;
config.title = "Bora Window";
config.message = "Hello, Bora! This is a message box.";


bora::logging::messageBox(config);

}