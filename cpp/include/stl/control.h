// Control Blocks for Unique and Shared Pointers
#pragma once
#include "stl/atomic.h"


namespace bora::stl {
template<typename T>
class ControlBlock {
public:
    T* ptr;
    u64 refCountId;

    ControlBlock(T* raw_ptr)
        : ptr(raw_ptr), refCountId(bora::stl::atomic::create(1)) {}

    void add_ref() {
        // Increment reference count manually
        u64 current = bora::stl::atomic::load(refCountId);
        bora::stl::atomic::store(refCountId, current + 1);
    }

    void release_ref() {
        while (true) {
            u64 current = atomic::load(refCountId); 
            if (current == 0) return;

         

            if (atomic::compare_exchange(refCountId, current, current - 1)) {
                if (current == 1) {
                    this->~ControlBlock();
                    delete ptr;
                }
                break;
            }
        }
    }

    u64 use_count() const {
        return bora::stl::atomic::load(refCountId);
    }
};

} // namespace bora::stl