#include "symbols/userWindow.h"
#include "symbols/graphics/graphics.h"
#include "userWindow.h"

namespace bora {

bnUserWindow::bnUserWindow(WindowConfig config) : configuration(config)
{ 
    windowOffset = createWindow(configuration, (u64*)&graphics);
}

bnUserWindow::~bnUserWindow()
{
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