#pragma once
#include <macros.h>

/// @brief Handles low level memory allocation and deallocation.
/// @note This is Low Level Memory Management, please use this system with proper care.
//       This is not a garbage collector, you must manage memory yourself.
namespace bora::memory {
IMPORT_ATTR("bora::memory", "alloc")
/// @brief Allocates memory of a specified size.
/// @param size The size of the memory to allocate in bytes.
/// @return A offset to the allocated memory.
extFunc u64 allocate(u64 size);

IMPORT_ATTR("bora::memory", "alloc_ptr")
/// @brief Allocates memory to your pointer
/// @param size The size of the memory to allocate in bytes.
/// @param ptr The pointer to store the allocated memory address. 
/// @note This is useful for changing a pointer to a new memory address with more memory.
/// @return A offset to the allocated memory.
extFunc u64 allocateWithPointer(u64 size, u64 ptr);
    
IMPORT_ATTR("bora::memory", "free")
/// @brief Frees memory that was previously allocated.
/// @param pointer A allocated memory pointer to free.
/// @note This function should only be called with pointers returned by `allocate`.
extFunc void deallocate(u64 pointer);

/// @brief Copies memory from source to destination.
/// @param dest The destination memory pointer.
/// @param src The source memory pointer.
/// @param size The number of bytes to copy.
/// @return The destination pointer.
inline void* copy(void* dest, const void* src, u64 size) {
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);
    for (u64 i = 0; i < size; ++i) {
        d[i] = s[i];
    }
    return dest;
}
}