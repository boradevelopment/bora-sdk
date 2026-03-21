// Apart of the BORA Runtime Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.
#include "BuildLibBoraCommand.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

void copySelectedHeaders(const fs::path& sourceDir, const fs::path& targetDir) {
    const std::string tag = "// BORA_INCLUDED_IN_SDK";

    for (const auto& entry : fs::recursive_directory_iterator(sourceDir)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            std::string firstLine;

            // We only need to check the very first line (or first few)
            if (std::getline(file, firstLine)) {
                if (firstLine.find(tag) != std::string::npos) {
                    // It's a match! Determine relative path to maintain folder structure
                    fs::path relative = fs::relative(entry.path(), sourceDir);
                    fs::path destination = targetDir / relative;

                    // Ensure the sub-directories exist in the destination
                    fs::create_directories(destination.parent_path());

                    // Copy the file (overwrite if exists)
                    fs::copy_file(entry.path(), destination, fs::copy_options::overwrite_existing);
                }
            }
        }
    }
}

CommandResult BuildLibBoraCommand::execute() {
    std::filesystem::path exePath;
    if(OsDependentPathGet(exePath) != CommandResult::Success){
        return CommandResult::Failure;
    }
    auto devRootPath = Environment::getEnvVar("BORA_DEV_ROOT_PATH");

    if(devRootPath.empty()){
        printf("BORA Source Root Path can not be found, in order to build libbora, please fork & follow the instructions at https://github.com/boradevelopment/bora");
        return CommandResult::DependencyMissing;
    }

    auto isDebug = AppParam::has("debug");

    std::string compPath(exePath.generic_string() + " ");
    compPath.append(devRootPath);
    compPath.append("/sdk/cpp/source/bora.cpp ");
    if(isDebug){
    compPath.append("-o \" ");
    compPath.append(devRootPath);
    compPath.append("/sdk/cpp/libs/bora\" ");
    compPath.append(" -as \"cdep\" --nobora");
    compPath.append(" --include=[\"");
    compPath.append(devRootPath);
    compPath.append("/global/cpp/contribs/");
    compPath.append("\"]");
    compPath.append(" -d");
    } else {
        fs::create_directories(devRootPath+"/sdk/cpp/bin/include");
        fs::create_directories(devRootPath+"/sdk/cpp/bin/libs");
        compPath.append("-o \" ");
        compPath.append(devRootPath);
        compPath.append("/sdk/cpp/bin/libs/bora\" ");
        compPath.append(" -as \"cdep\" --nobora");
        fs::copy(devRootPath+"/sdk/cpp/include", devRootPath+"/sdk/cpp/bin/include", fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        copySelectedHeaders(devRootPath+"/global/cpp/contribs", devRootPath+"/sdk/cpp/bin/include");
    }

    int command = Command(cmdApp, {{cmdCode, compPath}}).execute([](auto output){
    printf("%s\n", output.c_str());
});

    if(command != 0){
        return CommandResult::Failure;
    } else {
        return CommandResult::Success;
    }
}
