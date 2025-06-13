#pragma once
#include <cstdint>
#include "symbols/memory.h"

namespace bora::stl {

/// @brief Creates a shared pointer that manages the lifetime of an object.
/// A shared pointer is a smart pointer that retains shared ownership of an object through a pointer.
/// Multiple shared pointers can point to the same object, and the object is destroyed when the last shared pointer pointing to it is destroyed.
/// This implementation uses a reference count to keep track of how many shared pointers point to the same object.
/// @tparam T Your object type that you want to manage with a shared pointer.
template <typename T>
class shared_ptr {
private:
    T* ptr = nullptr;
    u64* ref_count = nullptr;

    void release() {
        if (ref_count) {
            if (--(*ref_count) == 0) {
                if (ptr) {
                    ptr->~T(); // Explicitly call destructor
                    bora::memory::deallocate(reinterpret_cast<uint64_t>(ptr));
                }
                bora::memory::deallocate(reinterpret_cast<uint64_t>(ref_count));
            }
        }
    }

public:
    shared_ptr() = default;

    explicit shared_ptr(T* raw) {
        ptr = raw;
        ref_count = reinterpret_cast<u64*>(
            bora::memory::allocate(sizeof(u64))
        );
        *ref_count = 1;
    }

    ~shared_ptr() {
        release();
    }

    shared_ptr(const shared_ptr& other) {
        ptr = other.ptr;
        ref_count = other.ref_count;
        if (ref_count) ++(*ref_count);
    }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    bool valid() const { return ptr != nullptr; }
};

} // namespace bora::stl
