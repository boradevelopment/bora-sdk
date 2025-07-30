#pragma once
#include <macros>

extern "C" {
IMPORT_ATTR("bora::stl::thread", "createThread")
int __create_thread(long func_ptr, int arg);
}
namespace bora::stl {
/// @brief A simple thread class that allows you to create threads in the Bora SDK.
/// @note This is a basic implementation and does not support advanced features like thread synchronization or joining.
/// This will be apart of the STL Major Update
class thread {
public:
    using func_t = void(*)(long);

    thread(func_t func, int arg = 0) {
        __create_thread((long)func, arg);
    }
};
} // namespace bora::stl
