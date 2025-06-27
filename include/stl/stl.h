// This is apart of the Bora SDK.
#pragma once

// This is your Standard Template Library (STL) namespace. Use this instead of C++ std for better optimization towards BORA.
namespace bora::stl {} 

#ifdef BORA_STL_USE_NAMESPACE // If you don't want to use bora::stl::bka, you can define BORA_STL_USE_NAMESPACE
    using namespace bora::stl;
#endif

#pragma region Bora SDK Includes
#ifndef B_STL_NEW
#include "new.h"
#endif
#ifndef B_STL_NOTHREAD
#include "thread.h"
#endif
#ifndef B_STL_NOSTRING
#include "string.h"
#endif
#ifndef B_STL_NOSHARED_POINTER
#include "shared_pointer.h"
#endif
#ifndef B_STL_NOUNIQUE_POINTER
#include "unique_pointer.h"
#endif
#ifndef B_STL_NOMOVE
#include "move.h"  
#endif
#ifndef B_STL_NOATOMIC
#include "atomic.h"
#endif
#ifndef B_STL_NOMAP
#include "map.h"
#endif
#pragma endregion