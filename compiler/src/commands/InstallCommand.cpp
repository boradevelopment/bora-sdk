// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include "InstallCommand.h"
#include <filesystem>
#if __linux__
#include <unistd.h>
#include <climits>
#endif

CommandResult InstallCommand::execute() {
    std::filesystem::path exePath;
    if(OsDependentPathGet(exePath) != CommandResult::Success){
        return CommandResult::Failure;
    }

    std::filesystem::path depsFolder(exePath.parent_path().string() + pathSlash +".deps");

    if(exists(depsFolder)){
        std::cout << "The dependencies folder already exists. Do you want to delete it and continue or leave it as is? (y/n): ";
        char response = 'n';
        std::cin >> response;

        if (response == 'y' || response == 'Y') {
            std::filesystem::remove_all(depsFolder);
            std::cout << "Folder deleted. Continuing...\n";
        } else {
            std::cout << "Leaving folder as is. Exiting.\n";
            return CommandResult::DependencyMissing;
        }
    }

    printf("Installing BORAC dependencies!\n");

    std::filesystem::create_directories(depsFolder);

#if WIN32
       SetFileAttributesA(depsFolder.string().c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_DIRECTORY);
#endif

    Command cloneGitEMCC(cmdApp, {{cmdCode, "git clone https://github.com/emscripten-core/emsdk.git "+depsFolder.string() + "/emsdk"}});

    if(cloneGitEMCC.execute() != 0){
        printf("I could not install emscripten repo. Check the logs\n");
        return CommandResult::DependencyMissing;
    }

    if(Command(cmdApp, {{cmdCode, depsFolder.string() + "/emsdk" + R"(/emsdk install latest)" }}).execute() != 0){
        printf("Emscripten failed to install it's tools, check logs\n");
        return CommandResult::DependencyMissing;
    }

    if(Command(cmdApp, {{cmdCode, depsFolder.string() + "/emsdk" + R"(/emsdk activate latest)"}}).execute() != 0){
        printf("Emscripten failed to activate it's tools, check logs\n");
        return CommandResult::DependencyMissing;
    }

    printf("Done! You can now compile your bora programs!\n");

    return CommandResult::Success;
}
