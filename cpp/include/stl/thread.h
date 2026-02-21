#pragma once
#include <macros>

extern "C" {
IMPORT_ATTR("bora::stl::thread", "createThread")
int __create_thread(u64 func_ptr, int arg);
IMPORT_ATTR("bora::stl::thread", "joinThread")
int __join_thread(u64 thread_id);
}

namespace bora::stl {
/// @brief A simple thread class that allows you to create threads in the Bora SDK.
/// @note This is a basic implementation and does not support advanced features like thread synchronization or joining.
/// This will be apart of the STL Major Update
class thread {
   using func_t = void(*)(long);
   u64 m_id;
public:
    thread(func_t func, int arg = 0) {
        m_id = __create_thread((long)func, arg);
    }

    void join() {
        if (m_id != -1) {
            __join_thread(m_id);
            m_id = -1;
        }
    }

    ~thread() {
        // In a real STL, you might detach or terminate here.
        // For a stress test, joining is safer.
        join(); 
    }
};
} // namespace bora::stl
