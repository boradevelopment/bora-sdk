#pragma once
#include <macros>
#include <cstdint>
#include "Command.h"

class CommandList {
public:
    CommandList() : mBuffer(""), name("") {
    };
    
    CommandList(const char* category, const char* name) : mBuffer(name), name(name) {
        mHandle = HostInitialize(category, name);
    }

    CommandList(const char* category, const char* name, const char* bufferName) : mBuffer(bufferName) {
        mHandle = HostInitialize(name);
    }
protected:
    IMPORT_ATTR("bora::commandList", "initialize")
    static u64 HostInitialize(const char* category, const char* name = "");

    IMPORT_ATTR("bora::commandList", "update")
    static void HostUpdate(u64 handle, u64 wasmPtr, u64 size);

    void Commit() {
        // Sync the current vector state to the host
        HostUpdate(mHandle, (u64)mBuffer.data.data, (u64)mBuffer.data.size);
    }

    CommandBuffer& GetBuffer() { return mBuffer; }
protected:
    u64 mHandle;
    CommandBuffer mBuffer;
    const char* name;
};