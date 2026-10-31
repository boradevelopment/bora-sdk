// #include "symbols/graphics/graphics.h"
// // #include "graphics.h"
// // #include "graphics.h"
// // #include "symbols/window.h"
// ResourceHandle<ITexture> *bnGraphics::CreateTextureCommand(const TextureDesc &d, const void *data, ResourceHandle<ITexture> *resource)
// {
//     return nullptr;
// }
// void bnGraphics::Submit()
// {
//     for (auto& cmd : commands){
//         buffer.Push(cmd);
//     }
    
// }

#include "symbols/graphics/graphics.h"

using namespace bora;

inline static std::unordered_map<const char*, bnGraphics*> guestGraphicsRegistry;

bnGraphics::bnGraphics(const char *windowID, const char *bufferID) : ICommandList(CommandCategories::NativeGraphics, windowID, bufferID)
{
        guestGraphicsRegistry[windowID] = this;
}

bnGraphics::~bnGraphics()
{
       guestGraphicsRegistry.erase(name);
}

bnGraphics *bnGraphics::findGraphicsFromWindowId(const char *windowID)
 
{
    if (windowID == nullptr) return nullptr;

    auto it = guestGraphicsRegistry.find(windowID);
    if (it != guestGraphicsRegistry.end()) {
        return it->second;
    } else {
            return nullptr;
    }
}