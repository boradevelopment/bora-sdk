// Apart of the BORA Runtime Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include <filesystem>
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

    auto projectDirectory = AppParam::getValue<std::string>("directory");
    if(projectDirectory.empty()){
        projectDirectory = std::filesystem::current_path().string();
    }
    auto buildDirectory = AppParam::getValue<std::string>("build");
    if(buildDirectory.empty()){
        buildDirectory = projectDirectory + R"(\build)";
    }

    std::filesystem::create_directories(buildDirectory);



    auto isDebug = AppParam::has("debug");

    // Build input file

    auto bwasmPath = std::filesystem::path(buildDirectory +"/bwasm.wasm");
    std::string compilerFlags = "-sMEMORY64=1 -Oz -sSTANDALONE_WASM -I\"" + sdkPath + "/include\" -o"+bwasmPath.string();
    std::string linkerFlags = "-Oz -Wl,--whole-archive \"" + sdkPath + "/libs/libbora.bcdep\" -Wl,--no-whole-archive -o"+bwasmPath.string();;

    if(isDebug){
        compilerFlags.append(" -g3");
    }



    std::string compPath = emBaseFolder.string() + "emcmake cmake -B \"" + buildDirectory + "\" \"" + projectDirectory + "\"";
    compPath += " -DCMAKE_CXX_FLAGS=\"" + compilerFlags + "\"";
    compPath += " -DCMAKE_EXE_LINKER_FLAGS=\"" + linkerFlags + "\"";
    compPath += " -DCMAKE_EXECUTABLE_SUFFIX_CXX=\".wasm\"";

    int makeResult = Command(cmdApp, {{cmdApp, compPath}}).execute([](auto output){
        printf("%s\n", output.c_str());
    });

    if(makeResult != 1){
        return CommandResult::Failure;
    }

    return CommandResult::Success;
}
