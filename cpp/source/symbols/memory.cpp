#include <macros>

namespace bora::memory {
void* copy(void* dest, const void* src, u64 size) {
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);
    for (u64 i = 0; i < size; ++i) {
        d[i] = s[i];
    }
    return dest;
}
}