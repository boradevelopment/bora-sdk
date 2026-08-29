#include <macros>
#include "memory.h"
#include <memory>
namespace bora::memory {
void deallocate(u64 pointer)
{
    // Call the global free from the C standard library
    ::free(reinterpret_cast<void*>(pointer));
}
u64 allocate(u64 size)
{
    return reinterpret_cast<u64>(::malloc(size));
}

u64 allocateWithPointer(u64 size, u64 ptr)
{
    // Typically used for reallocating an existing pointer to a new size
    return reinterpret_cast<u64>(::realloc(reinterpret_cast<void*>(ptr), size));
}

u64 move(u64 source, u64 sourceSize, u64 resultCapacity)
{
    // If resultCapacity is -1, we assume we just want enough for sourceSize
    u64 targetCapacity = (resultCapacity == (u64)-1) ? sourceSize : resultCapacity;
    
    // 1. Allocate new block
    u64 destination = allocate(targetCapacity);
    
    if (destination != 0 && source != 0) {
        // 2. Copy the data
        memcpy(reinterpret_cast<void*>(destination), 
               reinterpret_cast<void*>(source), 
               sourceSize);
        
        // 3. Free the old source
        deallocate(source);
    }
    
    return destination;
}

void* copy(void* dest, const void* src, u64 size) {
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);
    for (u64 i = 0; i < size; ++i) {
        d[i] = s[i];
    }
    return dest;
}
void* set(void* ptr, int value, u64 length) {
    unsigned char* p = static_cast<unsigned char*>(ptr);
    for (u64 i = 0; i < length; ++i) {
        p[i] = static_cast<unsigned char>(value);
    }
    return ptr;
}
}