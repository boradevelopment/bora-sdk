// Apart of the BORA Runtime Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include <filesystem>
#include "CmakeBuildCommand.h"
#include "V2Archive.h"

struct WasmFile {
    std::filesystem::path path;
    std::filesystem::file_time_type modifiedTime;
};


CommandResult CmakeBuildCommand::execute() {
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

    auto buildDirectory = AppParam::getValue<std::string>("build");
    if(buildDirectory.empty()){
        buildDirectory = std::filesystem::current_path().string() + R"(\build)";
    }


    auto logo = AppParam::getValue<std::string>("logo");
    if(logo.empty()){
        logo = R"(C:\Projects\bora\global\assets\logo\borat.png)";
    }

    std::string compPath = emBaseFolder.string() + "emmake make -C \"" + buildDirectory + "\" ";


    int makeResult = Command(cmdApp, {{cmdCode, compPath}}).execute([](auto output){
        printf("%s\n", output.c_str());
    });

    if(makeResult != 0){
        return CommandResult::Failure;
    }

    auto asParam = AppParam::getValue<std::string>("as");
    if(asParam.empty()) asParam = "app";

    std::vector<WasmFile> wasmFiles;

    bool isStatic = false;
    bool isRTDep = false;
    if(asParam == "app") {
        // I'll have to scan the folder for .wasm

        for (auto &entry: std::filesystem::recursive_directory_iterator(buildDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".wasm") {
                WasmFile wf{entry.path(), std::filesystem::last_write_time(entry)};
                wasmFiles.push_back(wf);
            }
        }

        if (wasmFiles.empty()) {
            std::cout << "Expected a wasm file to exist but it doesnt. Building as stactic library\n";

            for (auto &entry: std::filesystem::recursive_directory_iterator(buildDirectory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".a") {
                    isStatic = true;
                    WasmFile wf{entry.path(), std::filesystem::last_write_time(entry)};
                    wasmFiles.push_back(wf);
                }
            }

            if(wasmFiles.empty()){
                std::cout << "Nothing exists!\n";
                return CommandResult::DependencyMissing;
            }

        }


    } else if(asParam == "rdep"){
        isRTDep = true;
        for (auto &entry: std::filesystem::recursive_directory_iterator(buildDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".wasm") {
                WasmFile wf{entry.path(), std::filesystem::last_write_time(entry)};
                wasmFiles.push_back(wf);
            }
        }

        if (wasmFiles.empty()) {
            std::cout << "Expected a wasm file to exist but it doesnt.\n";
            return CommandResult::DependencyMissing;
        }
    }

    std::sort(wasmFiles.begin(), wasmFiles.end(), [](const WasmFile &a, const WasmFile &b) {
        return a.modifiedTime > b.modifiedTime;
    });

    auto output = std::filesystem::absolute(wasmFiles[0].path);


    if(!isStatic) {
        std::filesystem::path outputPath = output;
        outputPath.replace_extension(".bapp");

        {
            V2Archive archive(outputPath.filename().string(), outputPath.string(),
                              "This is a BORA Application! You shouldn't touch anything unless you know what you're doing...");
            archive.header.customVariables["id"] = "BORA";
            archive.header.customVariables["entry"] = output.filename().string();
            archive.addFile(output.string());
            archive.addFileAndGet(logo, "logo");
        }

        std::filesystem::remove(output);

        printf("Your BORA application is complete! You can find it at %s\n",
               std::filesystem::absolute(outputPath).string().c_str());
    } else {
        std::filesystem::path outputPath = output;
        outputPath.replace_extension(".bcdep");
        int buildArchive = Command(cmdApp, {{cmdCode, emBaseFolder.string() + "emar rcs "+outputPath.string() + " " + output.string()}}).execute([](auto output){
            printf("%s\n", output.c_str());
        });

        if(buildArchive != 0){
            printf("Please fix the following errors ^^^\n");
            return CommandResult::Failure;
        }

        std::filesystem::remove(output);

        printf("Your BORA Compile Dependency is complete! You can find it in %s", std::filesystem::absolute(outputPath).string().c_str());
    }

    if(isRTDep){
        std::filesystem::path outputPath = output;
        outputPath.replace_extension(".brdep");

        {
            V2Archive archive(outputPath.filename().string(), outputPath.string(),
                              "This is a BORA Application! You shouldn't touch anything unless you know what you're doing...");
            archive.header.customVariables["id"] = "BORADEP";
            archive.header.customVariables["depname"] = outputPath.stem().string();
            archive.addFile(output.string());
        }

        std::filesystem::remove(output);

        printf("Your BORA Runtime Dependency is complete! You can find it at %s\n",
               std::filesystem::absolute(outputPath).string().c_str());
    }

    return CommandResult::Success;
}
