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

    std::filesystem::path depsFolder(exePath.string() + "/.deps");

    if(exists(depsFolder)){ // Data exists! Reinstalling
        printf("Reinstalling BORAC dependencies so removing .deps\n");
        remove_all(depsFolder);
    } else printf("Installing BORAC dependencies!\n");

    std::filesystem::create_directories(depsFolder);

#if WIN32
       SetFileAttributesA(depsFolder.string().c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_DIRECTORY);
#endif

    printf("Installing emscripten dependency\n");

    // PLATFORM COMPATABILITY

    Command cloneGitEMCC(cmdApp, {{cmdCode, "git clone https://github.com/emscripten-core/emsdk.git "+depsFolder.string() + "/emsdk"}});

    printf("Installing emscripten git repository\n");

   if(cloneGitEMCC.execute([](auto output){
       printf("[Git Info]: %s\n", output.c_str());
   }) != 0){
       printf("I could not install emscripten repo. Check the logs\n");
       return CommandResult::DependencyMissing;
   }

   printf("Installing emscripten tools\n");
   if(Command(cmdApp, {{cmdCode, depsFolder.string() + "/emsdk" + R"(/emsdk install latest)" }}).execute([](auto output){
       printf("[Tool Info]: %s\n", output.c_str());
   }) != 0){
       printf("Emscripten failed to install it's tools, check logs\n");
       return CommandResult::DependencyMissing;
   }

    printf("Activating emscripten tools");
    if(Command(cmdApp, {{cmdCode, depsFolder.string() + "/emsdk" + R"(/emsdk activate latest)"}}).execute() != 0){
        printf("Emscripten failed to activate it's tools, check logs\n");
        return CommandResult::DependencyMissing;
    }

    printf("Done! You can now compile your bora programs!\n");

    return CommandResult::Success;
}
