#pragma once
#include "symbols/memory.h"
#include <macros.h>
#include "move.h"

namespace bora::stl {
template<typename T>
struct DefaultDeleter {
    void operator()(T* ptr) const {
        if (ptr) {
            ptr->~T(); // Explicit destructor call
            bora::memory::deallocate(reinterpret_cast<u64>(ptr));
        }
    }
};
/// @brief An unique pointer is a smart pointer that owns and manages another object through a pointer and disposes of that object when the unique pointer goes out of scope.
/// @tparam T The pointer type that the unique pointer will manage.
/// @tparam Deleter [Optional] A custom deleter type that defines how the object should be destroyed. If not provided, a default deleter will be used.
/// @note The unique pointer cannot be copied, but it can be moved. This means that ownership of the managed object can be transferred to another unique pointer, but there can only be one unique pointer managing a given object at a time.
template<typename T, typename Deleter = DefaultDeleter<T>>
class unique_ptr {
private:
    T* ptr = nullptr;
    Deleter deleter; 

public:
    // Constructors
    constexpr unique_ptr() noexcept : ptr(nullptr), deleter(Deleter()) {}
    explicit unique_ptr(T* raw_ptr) noexcept : ptr(raw_ptr), deleter(Deleter()) {}
    unique_ptr(T* raw_ptr, Deleter d) noexcept : ptr(raw_ptr), deleter(d) {}

    // No copy
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    // Move
    unique_ptr(unique_ptr&& other) noexcept : ptr(other.ptr), deleter(stl::move(other.deleter)) {
        other.ptr = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            deleter = stl::move(other.deleter);
            other.ptr = nullptr;
        }
        return *this;
    }

    ~unique_ptr() {
        reset();
    }

    // Accessors
    T* get() const noexcept { return ptr; }
    T* operator->() const noexcept { return ptr; }
    T& operator*() const noexcept { return *ptr; }

    explicit operator bool() const noexcept { return ptr != nullptr; }

    // Modifiers
    void reset(T* new_ptr = nullptr) noexcept {
        if (ptr) deleter(ptr);
        ptr = new_ptr;
    }

    T* release() noexcept {
        T* old_ptr = ptr;
        ptr = nullptr;
        return old_ptr;
    }
};
}