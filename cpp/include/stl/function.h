#pragma once
#include <cstdint>
#include <utility>
#include <type_traits>
#include <symbols/memory.h>

namespace bora::stl {

template<typename Signature>
class function; // forward declaration

template<typename... Args>
class function<void(Args...)> {
private:
    static constexpr size_t SBO_SIZE = sizeof(void*) * 3;
    static constexpr size_t SBO_ALIGN = alignof(void*);

    using storage_t = std::aligned_storage_t<SBO_SIZE, SBO_ALIGN>;

    struct callable_base {
        virtual void invoke(Args&&... args) = 0;
        virtual callable_base* clone(void* storage, bool& onHeap) const = 0;
        virtual void destroy(bool onHeap) = 0;
        virtual ~callable_base() = default;
    };

    template<typename F>
    struct callable_impl : callable_base {
        F f;
        callable_impl(F&& func) : f(std::forward<F>(func)) {}
        callable_impl(const F& func) : f(func) {}

        void invoke(Args&&... args) override { f(std::forward<Args>(args)...); }

        callable_base* clone(void* storage, bool& onHeap) const override {
            if (sizeof(callable_impl) <= SBO_SIZE) {
                // construct in-place
                callable_impl* ptr = new(storage) callable_impl(f);
                onHeap = false;
                return ptr;
            } else {
                // heap allocation
                callable_impl* ptr = new callable_impl(f);
                onHeap = true;
                return ptr;
            }
        }

        void destroy(bool onHeap) override {
            if (onHeap) delete this;
            else this->~callable_impl();
        }
    };

    callable_base* callable = nullptr;
    bool onHeap = false;
    storage_t sbo;

public:
    function() = default;

    template<typename F>
    function(F&& f) {
        using decayF = std::decay_t<F>;
        if (sizeof(callable_impl<decayF>) <= SBO_SIZE) {
            callable = new(&sbo) callable_impl<decayF>(std::forward<F>(f));
            onHeap = false;
        } else {
            callable = new callable_impl<decayF>(std::forward<F>(f));
            onHeap = true;
        }
    }

    // Copy constructor
    function(const function& other) {
        if (other.callable) {
            callable = other.callable->clone(&sbo, onHeap);
        }
    }

    // Move constructor
    function(function&& other) noexcept {
        callable = other.callable;
        onHeap = other.onHeap;
        if (!onHeap) {
            // Move SBO contents
            bora::memory::copy(&sbo, &other.sbo, SBO_SIZE);
            callable = reinterpret_cast<callable_base*>(&sbo);
        }
        other.callable = nullptr;
    }

    // Copy assignment
    function& operator=(const function& other) {
        if (this != &other) {
            if (callable) callable->destroy(onHeap);
            callable = other.callable ? other.callable->clone(&sbo, onHeap) : nullptr;
        }
        return *this;
    }

    // Move assignment
    function& operator=(function&& other) noexcept {
        if (this != &other) {
            if (callable) callable->destroy(onHeap);
            callable = other.callable;
            onHeap = other.onHeap;
            if (!onHeap) {
                bora::memory::copy(&sbo, &other.sbo, SBO_SIZE);
                callable = reinterpret_cast<callable_base*>(&sbo);
            }
            other.callable = nullptr;
        }
        return *this;
    }

    ~function() {
        if (callable) callable->destroy(onHeap);
    }

    void operator()(Args... args) {
        if (callable) callable->invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const { return callable != nullptr; }
};

} // namespace bora::stl
