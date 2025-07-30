#include "stl/string.h"

namespace bora::stl {
    string::string(const char* src) {
        len = strlen(src);
        capacity = len + 1;
        ptr = memory::allocate(capacity);
        memory::copy(reinterpret_cast<void*>(ptr), src, capacity);
    }
    string::~string() {
        if (ptr) {
            bora::memory::deallocate(ptr);
            capacity = 0;
            len = 0;
            ptr = 0;
        }
    }
    u64 string::strlen(const char* str) {
        u64 length = 0;
        while (str && str[length] != '\0') {
            ++length;
        }
        return length;
    }
    char* string::copy(char* dest, const char* src) {
        if (!dest || !src) return nullptr;
        char* ret = dest;
        while ((*dest++ = *src++));
        return ret;
    }

    int string::copy_s(char* dest, u64 destsz, const char* src) {
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
    u64 string::length() const {
        return len;
    }
    const char* string::c_str() const {
        return reinterpret_cast<const char*>(ptr);
    }
    
    char string::charAt(u64 index) const {
        if (index < 0 || index >= len) return 0;
        return *(reinterpret_cast<const char*>(ptr) + index);
    }

    void string::append(const char* suffix) {
    
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


    void string::reserve(u64 new_cap) {
    if (new_cap <= capacity) return;

    u64 new_ptr = bora::memory::allocate(new_cap);
    memory::copy(reinterpret_cast<void*>(new_ptr), reinterpret_cast<void*>(ptr), len + 1);
    bora::memory::deallocate(ptr);
    ptr = new_ptr;
    capacity = new_cap;
    }


    // Move semantics (recommended for resource classes)
    string::string(string&& other) noexcept {
        ptr = other.ptr;
        len = other.len;
        capacity = other.capacity;
        other.capacity = 0;
        other.ptr = 0;
        other.len = 0;
    }

    string& string::operator=(string&& other) noexcept {
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
}