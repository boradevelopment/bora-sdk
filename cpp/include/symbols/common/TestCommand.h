#include "symbols/common/CommandList.h"

// todo: synchronize global headers in SDK C++
#pragma pack(push, 1)
struct TestCommandData
{
    int value;
};
#pragma pack(pop)

struct TestCommandSDK : public ISDKCommand
{
    TestCommandData data;

    TestCommandSDK(int value)
    {
        data.value = value;
    }

    static constexpr const char* RuntimeName() { return "TestCommand"; }
    static constexpr const char* RuntimeCategory() { return "bnTests"; }
};

class TestCommandList : public CommandList {
public:
    TestCommandList() : CommandList("Test") {

    }

    void TestTheCommand(int value){
        auto sdkCommand = TestCommandSDK(value);
        mBuffer.Push(sdkCommand);
        Commit();
    }

    void JustPush(int value){
        auto sdkCommand = TestCommandSDK(value);
        mBuffer.Push(sdkCommand);
    }

    std::vector<u8>* GetData(){
        return &mBuffer.data;
    }

    void Clear(){
        mBuffer.data.clear();
        Commit();
    }

    void JustCommit(){
        Commit();
    }
};
