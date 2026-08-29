#include <cstddef>
#include <cstdint>
#include <macros>

extFunc {
struct CxaAtexitEntry {
void (*func)(void*);
void* arg;     
void* dso;      
};

// 256 per instance
constexpr size_t MAX_ATEXIT_ENTRIES = 256;
static CxaAtexitEntry g_atexit_table[MAX_ATEXIT_ENTRIES];
static size_t g_atexit_count = 0;


__attribute__((visibility("default")))
int __cxa_atexit(void (*func)(void*), void* arg, void* dso) {
    if (g_atexit_count >= MAX_ATEXIT_ENTRIES) {
        return -1; 
    }

    g_atexit_table[g_atexit_count].func = func;
    g_atexit_table[g_atexit_count].arg  = arg;
    g_atexit_table[g_atexit_count].dso  = dso;
    g_atexit_count++;

    return 0;
}

__attribute__((visibility("default")))
void __cxa_finalize(void* dso_handle) {
    for (size_t i = g_atexit_count; i > 0; --i) { // last will be executed first!
        size_t idx = i - 1;
        CxaAtexitEntry& entry = g_atexit_table[idx];

        if (entry.func != nullptr) {
            if (dso_handle == nullptr || entry.dso == dso_handle) {
                void (*dtor)(void*) = entry.func;
                void* arg = entry.arg;
                entry.func = nullptr;

                dtor(arg);
            }
        }
    }
}

__attribute__((export_name("__call_dtors")))
void bora_call_dtors() {
    __cxa_finalize(nullptr);
}

}