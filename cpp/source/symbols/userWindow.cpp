#include "symbols/userWindow.h"
#include "symbols/graphics/graphics.h"
#include "symbols/userWindow.h"

namespace bora {
inline static std::unordered_map<const char*, bnUserWindow*> guestWindowRegistry;

bnUserWindow::bnUserWindow(WindowConfig config) : configuration(config)
{ 
    guestWindowRegistry[(const char*)config.id] = this;
    windowOffset = createWindow(configuration.id, reinterpret_cast<void*>(config.update));
    graphics = bnGraphics(std::to_string(windowOffset).c_str(), std::to_string(windowOffset).c_str());
}

bnUserWindow::~bnUserWindow()
{
    guestWindowRegistry.erase(configuration.id);
    close();
}

void bnUserWindow::run()
{
    if (windowOffset != 0) {
     runWindow(windowOffset);
    }
}

void bnUserWindow::close()
{
    if (windowOffset != 0) {
     closeWindow(windowOffset);
     windowOffset = 0;
    }
}

bnGraphics* bnUserWindow::getGraphics()
{
    return &graphics;
}
}

bora::bnUserWindow* bora::bnUserWindow::getWindowObjectFromHandle(const char *wndID)
{
    if (wndID == nullptr) return nullptr;

    auto it = guestWindowRegistry.find(wndID);
    if (it != guestWindowRegistry.end()) {
        return it->second;
    } else {
        // Handle error: Host asked for a window that doesn't exist
        printf("Error: No graphics instance registered for ID: %s\n", wndID);
    }

    return nullptr;
}
