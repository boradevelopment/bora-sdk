// Allows you as the user to register your own command list systems for whatever reason
#pragma once
#include <macros>

namespace bora {
class ICommandBase {
public:
    virtual ~ICommandBase() = default;

    // Pure virtual functions for you to override
    virtual const char* GetCategory() const = 0;
    virtual const char* GetName() const = 0;
    virtual void Execute(void* payload) = 0;
};
}

#define BORA_REGISTER_COMMAND(CommandClassName) \
    static CommandClassName CommandClassName##_bora_cmd_instance; \
    /* Unique static thunk for this specific command class */ \
    __attribute__((used)) static void InternalExecute_##CommandClassName(void* payload) { \
    return CommandClassName##_bora_cmd_instance.Execute(payload); \
    } \
    __attribute__((constructor)) \
    void register_##CommandClassName() { \
        printf("register\n\n"); \
        bora::CommandListCommands::HostRegister( \
            CommandClassName##_bora_cmd_instance.GetCategory(), \
            CommandClassName##_bora_cmd_instance.GetName(), \
            reinterpret_cast<void*>(&InternalExecute_##CommandClassName) \
        ); \
    }