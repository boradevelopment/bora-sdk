#include "symbols/versioning.h"

namespace bora::versioning {
    namespace sdk {
    enum class Mode {
    Production = 0,
    Debug = 1,
    }; 

    // The full version as a C Style String
    constexpr const char* version = "0.0.1";
    // The major version integer for overhaul updates
    constexpr int versionMajor = 0;
    // The minor version integer for big updates but not enough to change systems
    constexpr int versionMinor = 0;
    // The patch version integer for small fixes and updates
    constexpr int versionPatch = 1;
    // Mode that the SDK was built in
    constexpr Mode mode = Mode::Production;

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