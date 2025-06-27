// Logging library: The project uses the "bora::logging" library for logging functionality.
// This library provides functions for creating message boxes and printing messages to the console.
#pragma once

#include <macros.h>
#include <stl/arguments.h>

/// @brief This will be your friend for logging and debugging.
/// It provides functions to create message boxes and print messages to the console.
/// @note Proper logging systems will be implemented in the future to replace other logging systems C++ offers.
namespace bora::logging {


 struct messageBoxConfig {
        const char* title;
         const char* message;
    };

IMPORT_ATTR("bora::logging", "msgbox")
/// @brief Creates a GUI Window [Global]
/// @param title A title
/// @param width A int width 
/// @param height A int height
/// @return BORA Window Object
extFunc int messageBox(messageBoxConfig config);

IMPORT_ATTR("bora::logging", "cout")
extFunc int print(const char* message);

IMPORT_ATTR("bora::logging", "cerr")
extFunc int printError(const char* message);

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
