// This is the core header file for the Bora SDK.
// Use this file to include all necessary components of the Bora SDK.
// If you don't want to include certain components, you can define the following B_NO* macros before including this header

#ifndef __cplusplus
#error "Bora SDK requires a C++ compiler. You're currently compiling with a C compiler."
#else
#pragma once
// This is the namespace for the Bora SDK. You can use this namespace to access all the components of the Bora SDK.
namespace bora {

}

#ifdef BORA_USE_NAMESPACE // If you don't want to use bora::bla, you can define BORA_USE_NAMESPACE
    using namespace bora;
#endif

#include "symbols/memory.h" // MEMORY IS REQUIRED ALWAYS!
#include "symbols/versioning.h"

#ifndef B_NOOPT_INCLUDE
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
#endif
#endif