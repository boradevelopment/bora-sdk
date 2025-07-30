#pragma once
#include "symbols/memory.h"
#include "stl/control.h"
#include <macros>

namespace bora::stl {
/// @brief Creates a shared pointer that manages the lifetime of an object.
/// A shared pointer is a smart pointer that retains shared ownership of an object through a pointer.
/// Multiple shared pointers can point to the same object, and the object is destroyed when the last shared pointer pointing to it is destroyed.
/// This implementation uses a reference count to keep track of how many shared pointers point to the same object.
/// @tparam T Your object type that you want to manage with a shared pointer.
template <typename T>
class shared_ptr {
private:
    bora::stl::ControlBlock<T>* control = nullptr;

    void release() {

        if (control) {
            control->release_ref();
            control = nullptr;
        }
    }

public:
    shared_ptr() = default;

    explicit shared_ptr(T* raw) {
        // void* mem = reinterpret_cast<void*>(memory::allocate(sizeof(ControlBlock<T>)));
        control = new ControlBlock<T>(raw);
    }

    ~shared_ptr() {
        release();
    }

shared_ptr(shared_ptr&& other) noexcept {
    control = other.control;
    other.control = nullptr;
}

// Move assignment
shared_ptr& operator=(shared_ptr&& other) noexcept {
    if (this != &other) {
        release();              // Release current control block
        control = other.control;
        other.control = nullptr;
    }
    return *this;
}

    T* get() const {
        return control ? control->ptr : nullptr;
    }

    T& operator*() const { return *get(); }
    T* operator->() const { return get(); }
    bool valid() const { return get() != nullptr; }

    u64 use_count() const {
        return control ? control->use_count() : 0;
    }
};
} // namespace bora::stl
