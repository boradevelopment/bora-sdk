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
            printf("Required: %llu", required);
            size_t new_capacity = required * 2;

            data = (u8*)bora::memory::move((u64)data, size, new_capacity);
            
            //  uint8_t* new_data = (uint8_t*)bora::memory::allocate(new_capacity);
            //  if(!new_data){
            //     printf("Invalid data");
            //     return;
            //  }

            // if(size > 0){
            //     memcpy(new_data, data, size);
            //     bora::memory::deallocate((u64)data);
            // }

            // data = new_data;
            // capacity = new_capacity;
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
       
        printf("capacity=%llu required=%llu offset=%llu\n", data.capacity, required, offset);
        data.resize(required);
        if (data.data == nullptr) return;
        auto* header = reinterpret_cast<SerializedCommandHeader*>(data.data + offset);
        printf("capacity=%llu size=%llu\n", data.capacity, data.size);
        if(header){
        header->nameHash = BoraHash64(ISDKCommand::RuntimeCategory(), strlen(ISDKCommand::RuntimeCategory()));
        header->nameHash = BoraHash64(ISDKCommand::RuntimeName(), strlen(ISDKCommand::RuntimeName()), header->nameHash);
        header->size = sizeof(DataType);
        }
        printf("hash=%llu size=%llu\n", header->nameHash, header->size);

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
