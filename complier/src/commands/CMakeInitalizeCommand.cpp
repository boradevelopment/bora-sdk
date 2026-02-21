// Apart of the BORA Runtime Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include <filesystem>
#include <Utilities.h>
#include "CMakeInitalizeCommand.h"
#if __linux__
#include <unistd.h>
#include <climits>
#endif
#if __APPLE__
#include <mach-o/dyld.h>
#endif

CommandResult CMakeInitalizeCommand::execute() {
    std::filesystem::path exePath;
    if(OsDependentPathGet(exePath) != CommandResult::Success){
        return CommandResult::Failure;
    }

    std::filesystem::path emBaseFolder(exePath.parent_path().string()+R"(\.deps\emsdk\upstream\emscripten\)");
    auto sdkPath = Environment::getEnvVar("BORA_SDK_PATH");

    if(sdkPath.empty()){
        printf("SDK can not be found, bora sdk was not installed properly");
        return CommandResult::DependencyMissing;
    }

    if(!exists(emBaseFolder)) return CommandResult::DependencyMissing;

    auto projectDirectory = AppParam::get("directory");
    if(projectDirectory.empty()){
        projectDirectory = std::filesystem::current_path().wstring();
    }
    auto buildDirectory = AppParam::get("build");
    if(buildDirectory.empty()){
        buildDirectory = projectDirectory + LR"(\build)";
    }

    std::filesystem::create_directories(buildDirectory);


    auto isDebug = AppParam::has("debug");

    // Build input file

    auto bwasmPath = std::filesystem::path(buildDirectory +L"/bwasm.wasm");
    std::string compilerFlags = "-sMEMORY64=1 -Oz -I\"" + sdkPath + "/include\" -o"+bwasmPath.string();
        std::string linkerFlags = "-Oz -Wl,--whole-archive \"" + sdkPath + "/libs/libbora.bcdep\" -Wl,--no-whole-archive -o"+bwasmPath.string();;

    if(isDebug){
        compilerFlags.append(" -g3");
    }



    std::string compPath = emBaseFolder.string() + "emcmake cmake -B \"" + wstringToUtf8(buildDirectory) + "\" \"" + wstringToUtf8(projectDirectory) + "\"";
    compPath += " -DCMAKE_CXX_FLAGS=\"" + compilerFlags + "\"";
    compPath += " -DCMAKE_EXE_LINKER_FLAGS=\"" + linkerFlags + "\"";
    compPath += " -DCMAKE_EXECUTABLE_SUFFIX_CXX=\".wasm\"";

    int makeResult = Command(cmdApp, {{cmdCode, compPath}}).execute([](auto output){
        printf("%s\n", output.c_str());
    });

    if(makeResult != 1){
        return CommandResult::Failure;
    }

    return CommandResult::Success;
}
