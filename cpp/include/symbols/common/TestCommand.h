#pragma once
#include "symbols/common/CommandList.h"
#include "nCommon/CommandListCategories.h"
#include "symbols/nMemory.h"
struct TestCommandSDK : public ISDKCommand
{
    TestCommandData data;
    
    TestCommandSDK(int value)
    {
        data.value = value;
        data.text = "This is from the BORA SDK";
        // data.ints[0] = value;
        // data.ints[1] = value+1;
    }

    static constexpr const char* RuntimeName() { return "TestCommand"; }
    static constexpr const char* RuntimeCategory() { return CommandCategories::NativeTesting; }
};

class TestCommandList : public CommandList {
public:
    TestCommandList() : CommandList(CommandCategories::NativeTesting, "Test") {
    //    mBuffer.data.reserve(4999 * (sizeof(SerializedCommandHeader) + sizeof(TestCommandData)));
    }

    void TestTheCommand(TestCommandSDK& value){
        mBuffer.Push(value);
        Commit();
    }

    void JustPush(TestCommandSDK& value){
        mBuffer.Push(value);
    }

    CommandBufferData GetData(){
        return mBuffer.data;
    }

    void Clear(){
        mBuffer.data.clear();
        Commit();
    }

    void JustCommit(){
        Commit();
    }
};
