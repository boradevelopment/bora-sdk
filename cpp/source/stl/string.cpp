#include "stl/string.h"

namespace bora::stl {
// ---------- Static helpers ----------
u64 string::cstrlen(const char* s) { u64 i = 0; while (s[i]) i++; return i; }
u64 string::cwstrlen(const wchar_t* s) { u64 i = 0; while (s[i]) i++; return i; }

void string::ensureCapacity(u64 additional) {
    u64 required = len + additional;
    if (required >= capacity) reserve((required + 1) * 2);
}

// ---------- Constructors & Destructor ----------
string::string() = default;

string::string(const char* src) {
    type = StringType::Char;
    len = cstrlen(src);
    capacity = len + 1;
    ptr = memory::allocate(capacity);
    copy_s(reinterpret_cast<char*>(ptr), capacity, src);
}

string::string(const wchar_t* src) {
    type = StringType::WChar;
    len = cwstrlen(src);
    capacity = len + 1;
    ptr = memory::allocate(capacity * sizeof(wchar_t));
    for (u64 i = 0; i < len; i++)
        reinterpret_cast<wchar_t*>(ptr)[i] = src[i];
}

string::string(const string& other) {
    type = other.type;
    len = other.len;
    capacity = other.capacity;
    ptr = memory::allocate(capacity * (type == StringType::Char ? sizeof(char) : sizeof(wchar_t)));
    if (type == StringType::Char)
        for (u64 i = 0; i < len; i++) reinterpret_cast<char*>(ptr)[i] = reinterpret_cast<char*>(other.ptr)[i];
    else
        for (u64 i = 0; i < len; i++) reinterpret_cast<wchar_t*>(ptr)[i] = reinterpret_cast<wchar_t*>(other.ptr)[i];
}

string::string(string&& other) noexcept
    : ptr(other.ptr), len(other.len), capacity(other.capacity), type(other.type) {
    other.ptr = 0; other.len = 0; other.capacity = 0;
}

string& string::operator=(const string& other) {
    if (this != &other) {
        if (ptr) memory::deallocate(ptr);
        type = other.type;
        len = other.len;
        capacity = other.capacity;
        ptr = memory::allocate(capacity * (type == StringType::Char ? sizeof(char) : sizeof(wchar_t)));
        if (type == StringType::Char)
            for (u64 i = 0; i < len; i++) reinterpret_cast<char*>(ptr)[i] = reinterpret_cast<char*>(other.ptr)[i];
        else
            for (u64 i = 0; i < len; i++) reinterpret_cast<wchar_t*>(ptr)[i] = reinterpret_cast<wchar_t*>(other.ptr)[i];
    }
    return *this;
}

string& string::operator=(string&& other) noexcept {
    if (this != &other) {
        if (ptr) memory::deallocate(ptr);
        ptr = other.ptr;
        len = other.len;
        capacity = other.capacity;
        type = other.type;
        other.ptr = 0; other.len = 0; other.capacity = 0;
    }
    return *this;
}

string::~string() { if (ptr) memory::deallocate(ptr); }

// ---------- Length & type ----------
u64 string::length() const { return len; }
StringType string::getType() const { return type; }
void string::clear() { len = 0; }

// ---------- Memory operations ----------
int string::copy_s(char* dest, u64 destsz, const char* src) {
    if (!dest || !src || destsz == 0) return -1;
    u64 slen = cstrlen(src);
    if (slen >= destsz) return -2;
    for (u64 i = 0; i <= slen; i++) dest[i] = src[i];
    return 0;
}

void string::reserve(u64 new_cap) {
    if (new_cap <= capacity) return;
    u64 newPtr = memory::allocate(new_cap * (type == StringType::Char ? sizeof(char) : sizeof(wchar_t)));
    if (type == StringType::Char)
        for (u64 i = 0; i < len; i++) reinterpret_cast<char*>(newPtr)[i] = reinterpret_cast<char*>(ptr)[i];
    else
        for (u64 i = 0; i < len; i++) reinterpret_cast<wchar_t*>(newPtr)[i] = reinterpret_cast<wchar_t*>(ptr)[i];
    memory::deallocate(ptr);
    ptr = newPtr;
    capacity = new_cap;
}

// ---------- Push back ----------
void string::push_back(char c) {
    ensureCapacity(1);
    if (type == StringType::Char) reinterpret_cast<char*>(ptr)[len++] = c;
    else reinterpret_cast<wchar_t*>(ptr)[len++] = (wchar_t)c;
}

void string::push_back(wchar_t wc) {
    ensureCapacity(1);
    if (type == StringType::Char) reinterpret_cast<char*>(ptr)[len++] = (char)wc;
    else reinterpret_cast<wchar_t*>(ptr)[len++] = wc;
}

// ---------- Append ----------
void string::append(const string& other) {
    ensureCapacity(other.len);
    if (type == StringType::Char) {
        char* dst = reinterpret_cast<char*>(ptr);
        if (other.type == StringType::Char) {
            char* src = reinterpret_cast<char*>(other.ptr);
            for (u64 i = 0; i < other.len; i++) dst[len + i] = src[i];
        } else {
            wchar_t* src = reinterpret_cast<wchar_t*>(other.ptr);
            for (u64 i = 0; i < other.len; i++) dst[len + i] = (char)src[i];
        }
    } else {
        wchar_t* dst = reinterpret_cast<wchar_t*>(ptr);
        if (other.type == StringType::Char) {
            char* src = reinterpret_cast<char*>(other.ptr);
            for (u64 i = 0; i < other.len; i++) dst[len + i] = (wchar_t)src[i];
        } else {
            wchar_t* src = reinterpret_cast<wchar_t*>(other.ptr);
            for (u64 i = 0; i < other.len; i++) dst[len + i] = src[i];
        }
    }
    len += other.len;
}

void string::append(const char* s) {
    u64 addLen = cstrlen(s);
    ensureCapacity(addLen);
    if (type == StringType::Char) {
        char* dst = reinterpret_cast<char*>(ptr);
        for (u64 i = 0; i < addLen; i++) dst[len + i] = s[i];
    } else {
        wchar_t* dst = reinterpret_cast<wchar_t*>(ptr);
        for (u64 i = 0; i < addLen; i++) dst[len + i] = (wchar_t)s[i];
    }
    len += addLen;
}

void string::append(const wchar_t* s) {
    u64 addLen = cwstrlen(s);
    ensureCapacity(addLen);
    if (type == StringType::Char) {
        char* dst = reinterpret_cast<char*>(ptr);
        for (u64 i = 0; i < addLen; i++) dst[len + i] = (char)s[i];
    } else {
        wchar_t* dst = reinterpret_cast<wchar_t*>(ptr);
        for (u64 i = 0; i < addLen; i++) dst[len + i] = s[i];
    }
    len += addLen;
}

// ---------- Operators ----------
string& string::operator+=(const string& other) { append(other); return *this; }
string& string::operator+=(const char* s) { append(s); return *this; }
string& string::operator+=(const wchar_t* s) { append(s); return *this; }

string operator+(const string& a, const string& b) {
    string result(a);
    result.append(b);
    return result;
}

string operator+(const char* a, const string& b) {
    string result(a);
    result.append(b);
    return result;
}

string operator+(const wchar_t* a, const string& b) {
    string result(a);
    result.append(b);
    return result;
}

// ---------- Access ----------
const void* string::c_str() const { return (void*)ptr; }
}
