#pragma once
#include <cstdint>
#include <cstring>
#include <macros.h>
#include "symbols/memory.h" // Provides bora::memory::allocate and ::free
#include <iostream>

namespace bora::stl {

/// A simple string class that manages a string in WASM memory.
class string {
private:
    u64 ptr = 0;       // Offset to string data in WASM memory
    u64 len = 0;       // Length of the string

public:
    string() = default;

    /// @brief This constructor initializes the string with a C-style string. 
    /// @note Programmer Details: This copies the content of the C-style string into WASM memory through memory::allocate, allocating enough space for the string and a null terminator.
    /// @param src 
    string(const char* src) {
        len = std::strlen(src);
        ptr = memory::allocate(len + 1); // +1 for null terminator
        std::memcpy(reinterpret_cast<void*>(ptr), src, len + 1);
    }

    ~string() {
        if (ptr) {
            bora::memory::deallocate(ptr);
            ptr = 0;
        }
    }

    u64 length() const {
        return len;
    }

    const char* c_str() const {
        return reinterpret_cast<const char*>(ptr);
    }

    char charAt(u64 index) const {
        if (index < 0 || index >= len) return 0;
        return *(reinterpret_cast<const char*>(ptr) + index);
    }

    void append(const char* suffix) {
        u64 suffix_len = std::strlen(suffix);
        u64 new_len = len + suffix_len;
        u64 new_ptr = bora::memory::allocate(new_len + 1); // +1 for null terminator

        // Copy existing content
        std::memcpy(reinterpret_cast<void*>(new_ptr), reinterpret_cast<void*>(ptr), len);

        // Append new content
        std::memcpy(reinterpret_cast<void*>(new_ptr + len), suffix, suffix_len + 1);

        // Free old memory
        bora::memory::deallocate(ptr);

        // Update internal state
        ptr = new_ptr;
        len = new_len;
    }

    // Move semantics (recommended for resource classes)
    string(string&& other) noexcept {
        ptr = other.ptr;
        len = other.len;
        other.ptr = 0;
        other.len = 0;
    }

    string& operator=(string&& other) noexcept {
        if (this != &other) {
            if (ptr) bora::memory::deallocate(ptr);
            ptr = other.ptr;
            len = other.len;
            other.ptr = 0;
            other.len = 0;
        }
        return *this;
    }

    // Disable copy (or implement deep copy if needed)
    string(const string&) = delete;
    string& operator=(const string&) = delete;
};

}
