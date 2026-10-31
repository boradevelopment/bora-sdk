#pragma once
#include <macros>
#include <cstdint>
#include "Command.h"

namespace bora {
    class CommandListCommands {
    public:
    IMPORT_ATTR("bora::commandList", "register")
    static void HostRegister(const char* category, const char* name, void* registerPtr);
    };

    template<template<typename...> class C>
    class ICommandList {
    public:
        ICommandList() : mBuffer(""), name("") {
        };
        
        ICommandList(const char* category, const char* name) : mBuffer(name), name(name) {
            mHandle = HostInitialize(category, name);
        }

        ICommandList(const char* category, const char* name, const char* bufferName) : mBuffer(bufferName) {
            mHandle = HostInitialize(name);
        }
    protected:
        IMPORT_ATTR("bora::commandList", "initialize")
        static u64 HostInitialize(const char* category, const char* name = "");

        IMPORT_ATTR("bora::commandList", "update")
        static void HostUpdate(u64 handle, u64 wasmPtr, u64 size);

        void Commit() {
            // Sync the current vector state to the host
            HostUpdate(mHandle, (u64)mBuffer.GetData(), (u64)mBuffer.size());
            mBuffer.Clear();
        }

        CommandBuffer<C<u8>>& GetBuffer() { 
            return mBuffer; 
        }
    protected:
        u64 mHandle;
        CommandBuffer<C<u8>> mBuffer;
        const char* name;
    };
}