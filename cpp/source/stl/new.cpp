#pragma once
#include "symbols/memory.h"

#ifndef __INTELLISENSE__
void* operator new(unsigned long size, void* ptr) noexcept { // I'm not including <new> 
    return ptr;
}
void operator delete(void*, void*) noexcept {}

void* operator new(unsigned long size) {
   return (void*)bora::memory::allocate(size);
}

void operator delete(void* ptr) noexcept {
     bora::memory::deallocate((u64)ptr);
}
#else
#include <new> // This is what intellisense wants but im not using STD in builds
#endif
