#define BORA_USE_NAMESPACE
#define BORA_STL_USE_NAMESPACE
#define B_HideStringWarnings
#include <bora>

int main() {
    logging::printf("bora sdk version: %s", bora::versioning::sdk::get_bora_sdk_version());
}
