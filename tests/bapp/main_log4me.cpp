#include <bora>

IMPORT_ATTR("log4me::log4me", "print")
extFunc void l4m_print(const char* message);

int main(){
    l4m_print("Hello Runtime Dependent!");

   // bora::versioning::sdk::get_bora_sdk_version();
}