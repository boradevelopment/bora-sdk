#pragma once

#include <macros>
#include "symbols/memory.h"

namespace bora::stl {
template <typename T>
class vector {
private:
    T* data_;
    u64 capacity_;
    u64 size_;

public:
    vector(u64 initial_capacity = 4)
        : capacity_(initial_capacity), size_(0) {
        data_ = (T*)memory::allocate(capacity_ * sizeof(T));
    }

    ~vector() {
        memory::deallocate((u64)data_);
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            resize(capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void resize(u64 new_capacity) {
        T* new_data = (T*)memory::allocate(new_capacity * sizeof(T));
        for (u64 i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        memory::deallocate((u64)data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    T* data() const { return data_; }
    u64 size() const { return size_; }

    T& operator[](u64 index) { return data_[index]; }
    const T& operator[](u64 index) const { return data_[index]; }
};
}
