#pragma once

#include <macros>


/// @brief This allows you to see the versions of both your sdk and runtime
namespace bora::versioning {
    /// @brief Get the version of the SDK you're using.
    namespace sdk {
    enum class Mode;

    const char* modeToString();
    
    extFunc {
    EXPORT_ATTR("get_bora_sdk_version")
    const char* get_bora_sdk_version();

    EXPORT_ATTR("get_bora_sdk_version_major")
    int get_bora_sdk_version_major();
    
    EXPORT_ATTR("get_bora_sdk_version_minor")
    int get_bora_sdk_version_minor();
    
    EXPORT_ATTR("get_bora_sdk_version_patch")
    int get_bora_sdk_version_patch();

    EXPORT_ATTR("get_bora_sdk_mode")
    Mode get_bora_sdk_mode();
    }

    }

    /// @brief Get the version of the runtime that is running your code.
    namespace runtime {

    }
}