#pragma once
#include <macros>
#include "symbols/memory.h" // Provides bora::memory::allocate and ::free

namespace bora::stl {
/// @brief Type of string
enum class StringType : u8 {
    Char  = 0,
    WChar = 1
};

/// @brief A string, this supports both wide characters and normal characters, conversion is needed though so becareful.
class string {
private:
    u64 ptr = 0;
    u64 len = 0;
    u64 capacity = 0;
    StringType type = StringType::Char;

    static u64 cstrlen(const char* s);
    static u64 cwstrlen(const wchar_t* s);
    void ensureCapacity(u64 additional);

public:
    // Constructors & Destructor
    string();
    string(const char* src);
    string(const wchar_t* src);
    string(const string& other);
    string(string&& other) noexcept;
    string& operator=(const string& other);
    string& operator=(string&& other) noexcept;
    ~string();

    // Length & type
    u64 length() const;
    StringType getType() const;
    void clear();

    // Memory operations
    static int copy_s(char* dest, u64 destsz, const char* src);
    void reserve(u64 new_cap);

    // Push back
    void push_back(char c);
    void push_back(wchar_t wc);

    // Append
    void append(const string& other);
    void append(const char* s);
    void append(const wchar_t* s);

    // Operators
    string& operator+=(const string& other);
    string& operator+=(const char* s);
    string& operator+=(const wchar_t* s);

    friend string operator+(const string& a, const string& b);
    friend string operator+(const char* a, const string& b);
    friend string operator+(const wchar_t* a, const string& b);

    // Access
    const void* c_str() const;
};
}