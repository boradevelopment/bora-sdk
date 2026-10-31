#include "log4me.h" 
#include <iostream>
#include <bora>
// KEY NOTE: [clang]
// Constructors priorities run from ascending order, first to last.
// Destructor priorities run for descending order, last to first.
ON_CONSTRUCTION_PRIORITY(101)
void log4me_init() {
    std::cout << "[log4me.wasm] First Dynamic library initialized via __wasm_call_ctors!" << std::endl;
}

ON_CONSTRUCTION_PRIORITY(102)
void log4me_init2() {
    std::cout << "[log4me.wasm] Second Dynamic library initialized via __wasm_call_ctors!" << std::endl;
}

ON_DESTRUCTION_PRIORITY(102)
void log4me_destroy() {
    std::cout << "[log4me.wasm] Goodbye!" << std::endl;
}

ON_DESTRUCTION_PRIORITY(101)
void log4me_destroy_2() {
    std::cout << "[log4me.wasm] Goodbye23!" << std::endl;
}



struct TestOnWasmSDK : public bora::ISDKCommand
{
    TestCommandData data;
    
    TestOnWasmSDK(int value)
    {
        data.value = value+5;
        data.text = "yessir.";
        // data.ints[0] = value;
        // data.ints[1] = value+1;
    }

    static constexpr const char* RuntimeName() { return "TestOnWasm"; }
    static constexpr const char* RuntimeCategory() { return CommandCategories::NativeTesting; }
};


class TestCommand : public bora::ICommandBase {
public:
    const char* GetCategory() const override { return CommandCategories::NativeTesting; }
    const char* GetName() const override { return "TestOnWasm"; }

    TestCommandData* data;

    void Execute(void* payload) override {

        data = reinterpret_cast<TestCommandData*>(payload);
        printf("%s - %d\n", data->text, data->value);
    }
};


BORA_REGISTER_COMMAND(TestCommand);


extFunc { 
#ifndef L4M_SHARED
LOG4ME_API("print") // Bora namespaces are formatted as $ rather than ::, there is no limit into how many namespaces you go specify.
void logOriginal(const char* data);

//Redirects
void log_print(const char* data){ // Implement your function as intended, nothing special.
    logOriginal(data);
}
#else
// Follow the same extFunc format
void log_print(const char* data){ // Implement your function as intended, nothing special.

    // printf("Based off log4me runtime dependent!\n"); // << HERE
    // printf("%s\n", data);


}
#endif
}
