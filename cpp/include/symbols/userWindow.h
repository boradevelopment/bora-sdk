#pragma once
#include <macros>
#include "stl/string.h"
#include "symbols/graphics/graphics.h"

namespace bora {
class bnWindow; 

enum SysButtonFlags : u8 {
    CLOSE = 1 << 0, // 0000 0001
    MINIMIZE = 1 << 1, // 0000 0010
    MAXIMIZE = 1 << 2, // 0000 0100
    ALL = CLOSE | MINIMIZE | MAXIMIZE
};

// Shown means that there is no hide logic by default, it will always be shown
// Hide UnFocus means that it will hide if the cursor isn't focusing on the titlebar in more than 3 seconds (if the titlebar show shortcut is pressed, it will be shown for 5 seconds)
// Hide Always means it will never been shown unless the application explicitly tells the titlebar to show or hide it.
enum TitleBarProperties : u8 {
    SHOWN = 1 << 0, // 0000 0001
    HIDEUNFOCUS = 1 << 1, // 0000 0010
    HIDEALWAYS = 1 << 2, // 0000 0100
    NO_LOGIC = 1 << 3,
    DEFAULT = SHOWN
};

struct bnWindowTitlebarConfig {
    float borderColor[3];
    float backgroundColor[3] = { 0, 0, 0 };
    float buttonHoverColor[3] = { 50, 50, 50 };
    float buttonPressedColor[3] = { 100, 100,100 };
    float closeButtonColor[3] = {180, 0, 0};
    float closeButtonPressedColor[3] = { 225, 0, 0 };
    u8 sysButtons = SysButtonFlags::ALL;
    u8 properties = TitleBarProperties::HIDEUNFOCUS;
    float hideSpeed = 8.5f;
    bool enabled = true;
};

typedef void (*UpdateProc)(const char* wndID, void* userObject, unsigned int msg, u64 wParam, intptr_t lParam);

struct WindowConfig {
    const char* id = "Window";
    UpdateProc update;
};

}   

extFunc {
IMPORT_ATTR("bora::window", "create")
u64 createWindow(const char* id, void* update);
IMPORT_ATTR("bora::window", "run")
void runWindow(u64 window);
IMPORT_ATTR("bora::window", "close")
void closeWindow(u64 window);
}

namespace bora {

// Creates a Bora Native Window
class bnUserWindow {
public:
    bnUserWindow(WindowConfig config);
    ~bnUserWindow();
public:
    void close();
    void run();
    bnGraphics* getGraphics();
    static bnUserWindow* getWindowObjectFromHandle(const char* wndID);
public:
    WindowConfig configuration; 
private:
    u64 windowOffset;
    bnGraphics graphics;
};
}