#pragma once

#include "symbols/common/CommandList.h"
#include "nCommon/CommandListCategories.h"
#include "symbols/nMemory.h"
#include "nGraphics/GraphicsAbstractions.h"
#include "nGraphics/GraphicsCommands.h"
#include <unordered_map>


struct CreateTextureCommandSDK : public ISDKCommand
{
    CreateTextureCommandData data;
    
    CreateTextureCommandSDK(const TextureDesc& d, const void* initialData, ResourceHandle<ITexture>* out) : data() {
        data.desc = d;
        data.initialData = initialData;
        data.handle = out;
    }

    static constexpr const char* RuntimeName() { return "CreateTextureCommand"; }
    static constexpr const char* RuntimeCategory() { return CommandCategories::NativeGraphics; }
};

class bnGraphics : public CommandList {
public:
    bnGraphics(){
        
    }
    
    bnGraphics(const char* windowID, const char* bufferID);

    ~bnGraphics();

    ResourceHandle<ITexture>* CreateTexture(const TextureDesc& desc, const void* initialData = nullptr, ResourceHandle<ITexture>* resource = nullptr) {
        ResourceHandle<ITexture>* tex;

        if (!resource) tex = new ResourceHandle<ITexture>();
        else tex = resource;

        tex->status = ResourceStatus::TO_BE_CREATED;
        mBuffer.Push(CreateTextureCommandSDK(desc, initialData, tex));
        return tex;
    }

    CommandBufferData GetData(){
        return mBuffer.data;
    }

    void Clear(){
        mBuffer.data.clear();
        Commit();
    }

    static bnGraphics* findGraphicsFromWindowId(const char* windowID);

#ifndef __BORA__STATIC
    EXPORT_ATTR("bora$graphics$bnGraphics$HostSubmit")
    static void HostSubmit(const char* windowID){
        // todo 
        if (windowID == nullptr) return;

        auto graphics = findGraphicsFromWindowId(windowID);
        if (graphics) {
            graphics->Commit();
        } else {
            // Handle error: Host asked for a window that doesn't exist
            printf("Error: No graphics instance registered for ID: %s\n", windowID);
        }
    }

    EXPORT_ATTR("bora$graphics$bnGraphics$HostClear")
      static void HostClear(const char* windowID){
        // todo
        if (windowID == nullptr) return;

        auto graphics = findGraphicsFromWindowId(windowID);
        if (graphics != nullptr) {
            graphics->Clear();
        } else {
            // Handle error: Host asked for a window that doesn't exist
            printf("Error: No graphics instance registered for ID: %s\n", windowID);
        }
    }
#endif
};

