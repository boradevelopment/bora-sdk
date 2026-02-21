#include "symbols/versioning.h"

namespace bora::versioning {
    namespace sdk {
    enum class Mode {
    Production = 0,
    Debug = 1,
    }; 

    // The full version as a C Style String
    constexpr const char* version = BSDK_VERSION;
    // The major version integer for overhaul updates
    constexpr int versionMajor = BSDK_VERSION_MAJOR;
    // The minor version integer for big updates but not enough to change systems
    constexpr int versionMinor = BSDK_VERSION_MINOR;
    // The patch version integer for small fixes and updates
    constexpr int versionPatch = BSDK_VERSION_PATCH;
    // Mode that the SDK was built in
    #ifdef DEBUG
    constexpr Mode mode = Mode::Debug;
    #else
    constexpr Mode mode = Mode::Production;
    #endif

    const char* modeToString() {
    switch (mode) {
        case Mode::Production: return "Production";
        case Mode::Debug: return "Debug";
    }
    return "Unknown";
    }   

    extFunc {
    const char* get_bora_sdk_version(){
        return version;
    }
    int get_bora_sdk_version_major(){
        return versionMajor;
    }
    int get_bora_sdk_version_minor(){
        return versionMinor;
    }
    int get_bora_sdk_version_patch(){
        return versionPatch;
    }
    Mode get_bora_sdk_mode(){
        return mode;
    }
}
    }
}