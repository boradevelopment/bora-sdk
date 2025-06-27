// This is NOT apart of the Bora SDK, but a macro header file for the Bora SDK.
// It may not be recommanded to include this file directly, but rather include the Bora SDK header file (bora.h).
#pragma once

#ifdef __INTELLISENSE__
#define IMPORT_ATTR(workspace, function) // C++ IntelliSense does not support Clang attributes for VSCODE [Do not worry, this is just for IntelliSense]
#else
#define IMPORT_ATTR(workspace, function) __attribute__((import_module(workspace), import_name(function)))
#endif

// Define a macro to shorten extern "C" for function declarations.
#define extFunc extern "C"
typedef signed int i32;
typedef unsigned int ui32;
typedef unsigned long long u64;
typedef unsigned char u8;


#ifndef __INTELLISENSE__
using namespace bora; // Intellisense is annoying
#endif
