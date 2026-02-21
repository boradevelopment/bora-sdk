// This is apart of the Bora SDK.
#pragma once

// This is your Standard Template Library (STL) namespace. Use this instead of C++ std for better optimization towards BORA.
namespace bora::stl {} 

#ifdef BORA_STL_USE_NAMESPACE // If you don't want to design your code bora::stl::bla, you can define BORA_STL_USE_NAMESPACE
    using namespace bora::stl;
#endif

#pragma region Bora SDK Includes
#ifndef B_STL_NOTHREAD
#include "thread.h"
#endif
#ifndef B_STL_NOSTRING
#include "string.h"
#endif
#pragma endregion