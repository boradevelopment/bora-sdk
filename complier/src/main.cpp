// Apart of the BORA SDK Compilier Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <cstdlib> // for _dupenv_s
#endif

void build_cpp_to_wasm(const std::string& cpp_file) {
    char* env_include = nullptr;

#ifdef _WIN32
    size_t env_size = 0;
    errno_t err = _dupenv_s(&env_include, &env_size, "BORA_SDK_INCLUDE");
    if (err || env_include == nullptr) {
        throw std::runtime_error("Environment variable BORA_SDK_INCLUDE not set");
    }
    std::string include_path(env_include);
    free(env_include);  // must free after copying
#else
    const char* env = std::getenv("MY_INCLUDE_PATH");
    if (!env) {
        throw std::runtime_error("Environment variable MY_INCLUDE_PATH not set");
    }
    std::string include_path(env);
#endif

    std::string clang = "./wasi/clang++";
    std::string sysroot = "./wasi/wasi-sysroot";
    std::string output_wasm = "final.wasm";

#ifdef _WIN32
   // clang += ".exe";
#endif

    std::string cmd = clang + " " + cpp_file +
                      " --target=wasm32-wasi -O2 " +
                      "--sysroot=" + sysroot +
                      " -I\"" + include_path + "\"" +
                      " -o " + output_wasm;

    std::cout << "Running: " << cmd << "\n";

    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        throw std::runtime_error("Clang failed to build WASM");
    }

    std::cout << "Success: Built " << output_wasm << "\n";
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: bora_compiler <source.cpp>\n";
        return 1;
    }

    try {
        build_cpp_to_wasm(argv[1]);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
