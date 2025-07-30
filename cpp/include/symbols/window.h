#pragma once
#include <macros>

// The Window Module allows you to create and manage GUI windows in the Bora SDK.
// This is not related to Win32 or any other platform-specific windowing system.
namespace bora::window {
extFunc {
IMPORT_ATTR("bora::window", "create_window")
/// @brief Creates a GUI Window [Global]
/// @param title A title
/// @param width A int width 
/// @param height A int height
/// @return BORA Window Object
[[deprecated("Why the hell did i do it like this?")]]
 int createWindow(const char *title, int width, int height);
}
}