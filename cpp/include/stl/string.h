#pragma once
#include <macros>
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
    string(const char* src);
    ~string();

    static u64 strlen(const char* str);

    #if !defined(B_HideStringCopyDeprecation) && !defined(B_HideStringWarnings)
    /// @brief Copies your character to the destination
    /// @param dest recieving end
    /// @param src sending end
    /// @note This may not safe, you should use copy_s
    [[deprecated("Use copy_s as copy can cause buffer overflows if not treated properly.")]]
    #endif
    static char* copy(char* dest, const char* src);

    /// @brief Copies your string safer than copy
    /// @param dest Destination (Recieving end)
    /// @param destsz Destination Size
    /// @param src Source (Sending end)
    /// @return Returns 0 if okay, returns -1 if dest, src or destsz are improper and returns -2 if src is longer than the size.
    static int copy_s(char* dest, u64 destsz, const char* src);

    u64 length() const;

    const char* c_str() const;

    char charAt(u64 index) const;

    void append(const char* suffix);

    void reserve(u64 new_cap);

    // Move semantics (recommended for resource classes)
    string(string&& other) noexcept;

    string& operator=(string&& other) noexcept;

    // Disable copy (or implement deep copy if needed)
    string(const string&) = delete;
    string& operator=(const string&) = delete;
};

}
