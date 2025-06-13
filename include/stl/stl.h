// This is apart of the Bora SDK.

// This is your Standard Template Library (STL) namespace. Use this instead of C++ std for better optimization towards BORA.
namespace bora::stl {} 

#pragma region Bora SDK Includes
#ifndef B_STL_NOTHREAD
#include "thread.h"
#endif
#ifndef B_STL_NOSTRING
#include "string.h"
#endif
#ifndef B_STL_NOSHARED_POINTER
#include "shared_pointer.h"
#endif
#ifndef B_STL_NOMOVE
#include "move.h"  
#endif
#ifndef B_STL_NOATOMIC
#include "atomic.h"
#endif
#pragma endregion