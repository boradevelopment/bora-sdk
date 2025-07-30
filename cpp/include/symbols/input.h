#pragma once
#include <macros>


/// @note Basic Input Namespace. Proper Input systems like keyboard, mouse and HID devices will be implemented in the future after STL is properly implemented.
namespace bora::input {
IMPORT_ATTR("bora::input", "cin")
extern "C" int cin(int ptr, int len);
}