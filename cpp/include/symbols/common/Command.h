#pragma once
#include <cstdint>
#include <vector>
#include <cstdio>
#include <cstring>
#include "nCommon/Hashing.h"
#include <iostream>
#include "symbols/memory.h"

struct ISDKCommand
{
public:
    virtual ~ISDKCommand() = default;
    SerializedCommandHeader header{};
};

struct CommandBufferData {
uint8_t* data = nullptr;
u64 capacity = 0;
u64 size = 0;

CommandBufferData(){

}

~CommandBufferData(){
    Destroy();
}

void resize(u64 required){
  if (required > capacity)
        {
            size_t new_capacity = required * 2;
            data = (u8*)bora::memory::move((u64)data, size, new_capacity);
        }
    size = required;
}


void clear(){
    size = 0;
}

void Destroy(){
    if (data)
    {
        bora::memory::deallocate((u64)data);
        data = nullptr;
    }

    capacity = 0;
    size = 0;
}
};

struct CommandBuffer
{

    CommandBuffer(const char* commandBufferID) : commandBufferID(commandBufferID) {

    }

    CommandBufferData data;

    template<typename ISDKCommand>
    void Push(ISDKCommand cmd)
    {
        using DataType = decltype(cmd.data);  
            
        u64 bytes_to_add =
        sizeof(SerializedCommandHeader) + sizeof(DataType);

        u64 offset = data.size; // Round up to nearest 8 bytes
        u64 required = offset + bytes_to_add;
       
        data.resize(required);
        if (data.data == nullptr) return;
        auto* header = reinterpret_cast<SerializedCommandHeader*>(data.data + offset);
        if(header){
        header->nameHash = BoraHash64(ISDKCommand::RuntimeCategory(), strlen(ISDKCommand::RuntimeCategory()));
        header->nameHash = BoraHash64(ISDKCommand::RuntimeName(), strlen(ISDKCommand::RuntimeName()), header->nameHash);
        header->size = sizeof(DataType);
        }
        // Copy only the data struct
        memcpy(data.data + offset + sizeof(SerializedCommandHeader), &cmd.data, sizeof(DataType));
         data.size = required;
    }

    // todo: add this.
    void HostClear(const char* commandBufferID)
    {

    }

    private:
    const char* commandBufferID;
};
