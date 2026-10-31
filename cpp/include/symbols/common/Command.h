#pragma once
#include <cstdint>
#include <vector>
#include <cstdio>
#include <cstring>
#include "nCommon/Hashing.h"
#include "nCommon/CommandListContainers.h"
#include <iostream>
#include "symbols/memory.h"

namespace bora {
struct ISDKCommand
{
public:
    virtual ~ISDKCommand() = default;
    SerializedCommandHeader header{};
};

template<CommandListContainer C>
struct CommandBuffer {
    // Construct with an ID and an optional container instance (e.g., ThreadLocal or Mutex container)
    CommandBuffer(const char* commandBufferID, C container = C{})
        : commandBufferID(commandBufferID), container(std::move(container)) {}

    template<typename ICommand>
    void Push(const ICommand& cmd)
    {
        using DataType = decltype(cmd.data);

        SerializedCommandHeader header{};
        header.nameHash = BoraHash64(ICommand::RuntimeCategory(), strlen(ICommand::RuntimeCategory()));
        header.nameHash = BoraHash64(ICommand::RuntimeName(), strlen(ICommand::RuntimeName()), header.nameHash);
        header.size = sizeof(DataType);

        constexpr std::size_t totalSize = sizeof(SerializedCommandHeader) + sizeof(DataType);
        std::array<u8, totalSize> flatBuffer;
        
        std::memcpy(flatBuffer.data(), &header, sizeof(SerializedCommandHeader));
        std::memcpy(flatBuffer.data() + sizeof(SerializedCommandHeader), &cmd.data, sizeof(DataType));
        container.push_back(flatBuffer.data(), flatBuffer.size());
    }

    void Clear()
    {
       container.clear();
    }

    const void* GetData() const {
        return static_cast<const void*>(container.data());
    }

    void* GetData() {
        return static_cast<void*>(container.data());
    }

    std::size_t GetDataSize() const {
        return container.size() * sizeof(typename C::value_type);
    }   

    std::size_t size() const {
        return container.size();
    }

private:
     C container;
    const char* commandBufferID;
};
}