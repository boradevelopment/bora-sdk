#pragma once
#include <macros.h>
#ifndef __INTELLISENSE__
inline void* operator new(unsigned long size, void* ptr) noexcept { // I'm not including <new> 
    return ptr;
}
inline void operator delete(void*, void*) noexcept {}
#else
#include <new> // This is what intellisense wants but im not using STD in builds
#endif
