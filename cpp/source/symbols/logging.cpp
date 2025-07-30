#include "symbols/logging.h"
#include <stl/arguments.h>

namespace bora::logging {
    void errorf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024]; // Adjust size as needed
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    printError(buffer);
    }

    void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024]; // Adjust size as needed
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    print(buffer);
    }

    const char* getFormattedString(const char* format, ...) {
    static char buffer[1024]; // Static to keep it alive after function returns
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return buffer;
    } 
}