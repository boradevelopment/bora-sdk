// Logging library: The project uses the "bora::logging" library for logging functionality.
// This library provides functions for creating message boxes and printing messages to the console.
#pragma once

#include <macros>

/// @brief This will be your friend for logging and debugging.
/// It provides functions to create message boxes and print messages to the console.
/// @note Proper logging systems will be implemented in the future to replace other logging systems C++ offers.
namespace bora::logging {

enum class MessageBoxType : u64 {
        NONE = 0,
        INFORMATION = 1,
        WARNING = 2,
        PROBLEM = 3
};
struct messageBoxConfig {
        u64 title;
        u64 message;
        MessageBoxType type;

        messageBoxConfig(const char* title, const char* msg, logging::MessageBoxType t)
        : title(reinterpret_cast<u64>(title)),
          message(reinterpret_cast<u64>(msg)),
          type(t) {}
};

IMPORT_ATTR("bora::logging", "msgbox")
/// @brief Creates a GUI Window [Global]
/// @param title A title [as pointer]
/// @param width A int width [as pointer]
/// @param height A int height
/// @return BORA Window Object
extFunc int messageBox(messageBoxConfig config);

IMPORT_ATTR("bora::logging", "cout")
extFunc int print(const char* message);

IMPORT_ATTR("bora::logging", "cerr")
extFunc int printError(const char* message);

void errorf(const char* format, ...);

void printf(const char* format, ...);

const char* getFormattedString(const char* format, ...);

}
