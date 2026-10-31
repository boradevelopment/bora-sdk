#pragma once
#include <macros>
#include <chrono>



namespace bora {
namespace private_thread { // private namespace so it doesn't appear in intellisense.
IMPORT_ATTR("bora::thread", "createThread")
int __create_thread(u64 func_ptr, u8* argumentArrayData, u64 argumentArraySize);
IMPORT_ATTR("bora::thread", "joinThread")
int __join_thread(int thread_id);
IMPORT_ATTR("bora::thread", "setThreadName")
void __name_thread(int thread_id, u64* namePtr);
IMPORT_ATTR("bora::thread", "interruptThread")
void __interrupt_thread(int thread_id);
IMPORT_ATTR("bora::thread", "resumeThread")
void __resume_thread(int thread_id);
IMPORT_ATTR("bora::thread", "killThread")
void __kill_thread(int thread_id); 
IMPORT_ATTR("bora::thread", "detachThread")
void __detach_thread(int thread_id); 
IMPORT_ATTR("bora::thread", "getThreadName")
u64 __get_thread_name(int thread_id);
}

/**
 * @brief Provides an API for managing the current thread's execution state,
 * time delays, and scheduling hints, pretyt much a copy of the std::this_thread.
 */
class this_thread {
public:
    /**
     * @brief Suspends execution of the current thread for a specified duration.
     * @param milliseconds The number of milliseconds to sleep.
     */
    IMPORT_ATTR("bora::thread", "sleepFor")
    static void sleep_for(u64 milliseconds);

    /**
     * @brief Suspends execution of the current thread until a specified absolute timestamp.
     * @param absoluteTimestampMs The absolute target timestamp in milliseconds.
     */
    IMPORT_ATTR("bora::thread", "sleepUntil")
    static void sleep_until(u64 absoluteTimestampMs);

    /**
     * @brief Reschedules the execution of threads, allowing other threads to run.
     */
    IMPORT_ATTR("bora::thread", "yield")
    static void yield();

    /**
     * @brief Get's the ID of the thread.
     */
    IMPORT_ATTR("bora::thread", "getThreadId")
    static i64 id();

    static const char* name(){
        return (const char*)private_thread::__get_thread_name(id());
    }

    /**
     * @brief Suspends execution of the current thread for a standard C++ duration.
     * @tparam Rep An arithmetic type representing the number of ticks.
     * @tparam Period A std::ratio representing the tick period (e.g., std::chrono::seconds).
     * @param duration The time span to sleep for.
     */
    template <typename Rep, typename Period>
    static void sleep_for(std::chrono::duration<Rep, Period> duration) {
        u64 ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        sleep_for(ms);
    }

    /**
     * @brief Suspends execution of the current thread until a standard C++ time point.
     * @tparam Clock The clock on which this time point is measured.
     * @tparam Duration A std::chrono::duration representing the time since epoch.
     * @param timePoint The absolute time point until which to sleep.
     */
    template <typename Clock, typename Duration>
    static void sleep_until(std::chrono::time_point<Clock, Duration> timePoint) {
        u64 ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            timePoint.time_since_epoch()
        ).count();
        sleep_until(ms);
    }
};

/// @brief A simple thread class that allows you to create threads in the Bora SDK.
/// @note This is a basic implementation and does not support advanced features like thread synchronization or joining.
/// This will be apart of the STL Major Update
// template<>
class thread {
private:

    using func_t = void(*)(long);
    int m_id;
    bool m_joinable = true;
    std::shared_ptr<std::vector<uint8_t>> payload;

    template <typename T>
    static void append_bytes(std::vector<uint8_t>& buffer, const T& val) {
            constexpr size_t alignment = alignof(T);
        size_t current_size = buffer.size();
        size_t padding = (alignment - (current_size % alignment)) % alignment;
        buffer.resize(current_size + padding);

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&val);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
    }

    class ArgumentReader {
    const uint8_t* m_ptr;
    const uint8_t* m_end;

public:
    ArgumentReader(const uint8_t* data, size_t size) 
        : m_ptr(data), m_end(data + size) {}

    // Read a single trivial/POD type out of the stream
    template<typename T>
    T read() { // we need to do this aligned!
    if (m_ptr + sizeof(T) > m_end) {
        throw std::runtime_error("Out of bounds while reading thread arguments!");
    }

    uintptr_t current_addr = reinterpret_cast<uintptr_t>(m_ptr);
    size_t alignment = alignof(T);
    size_t padding = (alignment - (current_addr % alignment)) % alignment;

    const uint8_t* aligned_ptr = m_ptr + padding;
    if (aligned_ptr + sizeof(T) > m_end) {
        throw std::runtime_error("Out of bounds while reading thread arguments!");
    }

    m_ptr = aligned_ptr;

    if constexpr (std::is_trivially_copyable_v<T>) {
        T value;
        std::memcpy(&value, m_ptr, sizeof(T));
        m_ptr += sizeof(T);
        return value;
    } else {
        T value = *reinterpret_cast<const T*>(m_ptr);
        m_ptr += sizeof(T);
        return value;
    }
    }

    };

    template<typename _Callable, typename... _Args>
    static void thread_trampoline(const uint8_t* data, size_t size) {
        ArgumentReader reader(data, size);
        _Callable func = reader.template read<_Callable>();

        auto args = std::tuple<std::decay_t<_Args>...>{ 
            reader.template read<std::decay_t<_Args>>()... 
        };

        std::apply(std::move(func), std::move(args));
    }
public:
    template<typename _Callable, typename... _Args>
    thread(_Callable&& func, _Args&&... arguments) {
    static_assert(std::is_default_constructible_v<std::decay_t<_Callable>>, 
    "bora::thread requires a stateless lambda (empty capture list []).");

    static_assert((sizeof...(_Args) == 0) || (std::is_trivial_v<std::decay_t<_Args>> || ...), 
        "All arguments passed to bora::thread must be trivial/trivially copyable types (e.g., pointers, primitives).");

    payload = std::make_shared<std::vector<uint8_t>>();
    append_bytes(*payload, std::forward<_Callable>(func));    
    (append_bytes(*payload, std::forward<_Args>(arguments)), ...);

    m_id = private_thread::__create_thread((u64)&thread_trampoline<_Callable, _Args...>, payload->data(), payload->size());
    if(m_id == -1){
        // todo: error check
    }
    }

    void join() {
        if (m_id != -1 && m_joinable) {
            private_thread::__join_thread(m_id);
            m_id = -1;
        }
    }

    void interrupt() {
        if (m_id != -1) {
            private_thread::__interrupt_thread(m_id);
        }
    }

    void resume() {
        if (m_id != -1) {
            private_thread::__resume_thread(m_id);
        }
    }

    void kill() {
        if (m_id != -1) {
            private_thread::__kill_thread(m_id);
            m_id = -1;
        }
    }

    void setName(const char* name) {
        if (m_id != -1) {
            private_thread::__name_thread(m_id, (u64*)name);
        }
    }

    const char* getName() {
        if (m_id != -1) {
            return (const char*)private_thread::__get_thread_name(m_id);
        }

        return nullptr;
    }

    bool joinable(){
        return m_joinable;
    }

    void detach(){
        if(m_joinable){
            m_joinable = false;
            private_thread::__detach_thread(m_id);
        }
    }

    ~thread() {
        join(); 
    }
};
} // namespace bora::stl
