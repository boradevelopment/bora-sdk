#pragma once
#include <macros.h>

extern "C" {
IMPORT_ATTR("bora::stl::thread", "createThread")
int __create_thread(int func_ptr, int arg);
}
namespace bora::stl {
/// @brief A simple thread class that allows you to create threads in the Bora SDK.
/// @note This is a basic implementation and does not support advanced features like thread synchronization or joining.
/// This will be apart of the STL Major Update
class thread {
public:
    using func_t = void(*)(int);

    thread(func_t func, int arg = 0) {
        __create_thread((int)func, arg);
    }
};
} // namespace bora::stl
