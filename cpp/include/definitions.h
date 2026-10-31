// Manages the module's identity in order for the registry to work on the host.
// The ID is usually managed by the compilier itself, with it handing over an APP ID preprocessor definition.
// This is only included in a runtime application or dependency, this is not available inside the compiled dependency.
#ifndef __BORA__COMPILED
#define BORA_STRINGIFY_IMPL(x) #x
#define BORA_STRINGIFY(x) BORA_STRINGIFY_IMPL(x)

#ifndef __BORA__ID // is this misleading?
#define __BORA__ID ""
#endif
static_assert(sizeof(BORA_STRINGIFY(__BORA__ID)) > 1, "No App ID available. This possibly means that you haven't given a proper identifiable name for your application. If not, this is a compilier issue.");

__attribute__((export_name("module_id")))
const char* bora_module_id() {
    return BORA_STRINGIFY(__BORA__ID);
}
#endif