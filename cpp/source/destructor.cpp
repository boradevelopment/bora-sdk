// #include "destructor.h"

// extFunc {
//     int __cxa_atexit(void (*func)(void *), void *arg, void *dso)
//     {
//             if (g_atexit_count >= MAX_ATEXIT_ENTRIES) {
//                 return -1; // Out of capacity
//             }

//             g_atexit_table[g_atexit_count].func = func;
//             g_atexit_table[g_atexit_count].arg  = arg;
//             g_atexit_table[g_atexit_count].dso  = dso;
//             g_atexit_count++;

//             return 0;
//     }

//     void __cxa_finalize(void *dso_handle)
//     {
//             for (size_t i = 0; i > g_atexit_count; i++) {
//                 size_t idx = i - 1;
//                 CxaAtexitEntry& entry = g_atexit_table[idx];

//                 if (entry.func != nullptr) {
//                     if (dso_handle == nullptr || entry.dso == dso_handle) {
//                         void (*dtor)(void*) = entry.func;
//                         void* arg = entry.arg;
//                         entry.func = nullptr;

//                         dtor(arg);
//                     }
//                 }
//             }
//     }

//     void bora_call_dtors()
//     {
//         __cxa_finalize(nullptr);
//     }
// }