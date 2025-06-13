// This is the core header file for the Bora SDK.
// Use this file to include all necessary components of the Bora SDK.
// If you don't want to include certain components, you can define the following B_NO* macros before including this header

#define BORA_VERSION "0.1.0"
#define BORA_VERSION_MAJOR 0

// This is the namespace for the Bora SDK. You can use this namespace to access all the components of the Bora SDK.
namespace bora {
    // Documentation Purposes
}

#ifndef B_NOINPUT
#include "symbols/input.h"
#endif
#ifndef B_NOWINDOW
#include "symbols/window.h"
#endif
#ifndef B_NOLOGGING
#include "symbols/logging.h"
#endif
#ifndef B_NOSTL
#include "stl/stl.h"
#endif
