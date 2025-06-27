#pragma once
#include <macros.h>
#include "symbols/memory.h" // Provides bora::memory::allocate and ::free

namespace bora::stl {

/// A simple string class that manages a string in WASM memory.
class string {
private:
    u64 ptr = 0;       // Offset to string data in WASM memory
    u64 len = 0;       // Length of the string
    u64 capacity = 0;

public:
    string() = default;

    /// @brief This constructor initializes the string with a C-style string. 
    /// @note Programmer Details: This copies the content of the C-style string into WASM memory through memory::allocate, allocating enough space for the string and a null terminator.
    /// @param src 
    string(const char* src) {
        len = strlen(src);
        capacity = len + 1;
        ptr = memory::allocate(capacity);
        memory::copy(reinterpret_cast<void*>(ptr), src, capacity);
    }
    ~string() {
        if (ptr) {
            bora::memory::deallocate(ptr);
            capacity = 0;
            len = 0;
            ptr = 0;
        }
    }

    static u64 strlen(const char* str) {
        u64 length = 0;
        while (str && str[length] != '\0') {
            ++length;
        }
        return length;
    }
    
    // What the hell intellisense? - I have to put documentation inside of the if statement?
    #if !defined(B_HideStringCopyDeprecation) && !defined(B_HideStringWarnings)
    /// @brief Copies your character to the destination
    /// @param dest recieving end
    /// @param src sending end
    /// @note This is not safe, you should use copy_s
    [[deprecated("Use copy_s as copy can cause buffer overflows if not treated properly.")]]
    #endif
    static char* copy(char* dest, const char* src) {
        if (!dest || !src) return nullptr;
        char* ret = dest;
        while ((*dest++ = *src++));
        return ret;
    }

    /// @brief Copies your string safer than copy
    /// @param dest Destination (Recieving end)
    /// @param destsz Destination Size
    /// @param src Source (Sending end)
    /// @return Returns 0 if okay, returns -1 if dest, src or destsz are improper and returns -2 if src is longer than the size.
    static int copy_s(char* dest, u64 destsz, const char* src) {
        if (!dest || !src || destsz == 0) return -1;
        u64 i = 0;
        for (; i < destsz - 1 && src[i] != '\0'; ++i) {
            dest[i] = src[i];
        }
        dest[i] = '\0';
        // If src was longer than destsz-1, return error
        if (src[i] != '\0') return -2;
        return 0;
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
    
    u64 suffix_len = strlen(suffix);
    u64 new_len = len + suffix_len;

    if (new_len + 1 > capacity) {
        // over-allocate to avoid frequent reallocations
        capacity = (new_len + 1) * 1.5;
        u64 new_ptr = bora::memory::allocate(capacity);
        memory::copy(reinterpret_cast<void*>(new_ptr), reinterpret_cast<void*>(ptr), len);
        bora::memory::deallocate(ptr);
        ptr = new_ptr;
    }

    memory::copy(reinterpret_cast<void*>(ptr + len), suffix, suffix_len + 1); // includes null terminator
    len = new_len;
}


    void reserve(u64 new_cap) {
    if (new_cap <= capacity) return;

    u64 new_ptr = bora::memory::allocate(new_cap);
    memory::copy(reinterpret_cast<void*>(new_ptr), reinterpret_cast<void*>(ptr), len + 1);
    bora::memory::deallocate(ptr);
    ptr = new_ptr;
    capacity = new_cap;
    }


    // Move semantics (recommended for resource classes)
    string(string&& other) noexcept {
        ptr = other.ptr;
        len = other.len;
        capacity = other.capacity;
        other.capacity = 0;
        other.ptr = 0;
        other.len = 0;
    }

    string& operator=(string&& other) noexcept {
        if (this != &other) {
            if (ptr) bora::memory::deallocate(ptr);
            ptr = other.ptr;
            len = other.len;
            capacity = other.capacity;
            other.capacity = 0;
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
