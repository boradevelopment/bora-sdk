#pragma once
#if __INTELLISENSE__
#define va_start(ap, param)

#define va_end(ap)
#define va_arg(ap, type)
#define vsnprintf(buf, size, format, args)

typedef void* va_list;
#else
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
/* C23 uses a special builtin. */
#define va_start(...) __builtin_c23_va_start(__VA_ARGS__)
#else
/* Versions before C23 do require the second parameter. */
#define va_start(ap, param) __builtin_va_start(ap, param)
#endif
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define vsnprintf(buf, size, format, args) __builtin_vsnprintf(buf, size, format, args)

typedef __builtin_va_list va_list;
#endif
