// Apart of the BORA SDK Compilier Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include "TAZA.h"
#include "tools/Command.h"
#include "tools/AppParam.h"

#include "commands/BuildCommand.h"
#include "commands/InstallCommand.h"
#include "commands/CMakeInitalizeCommand.h"
#include "commands/CmakeBuildCommand.h"

int main(int argc, char** argv) {

    AppParam::registerParam("as");
    AppParam::registerParam("output", {"-o"});
    AppParam::registerParam("nobora", {"-nb"});
    AppParam::registerParam("logo", {"-lg"});
    AppParam::registerParam("debug", {"-d"});
    AppParam::registerParam("directory", {"-dir"});
    AppParam::registerParam("build", {"-b", "-B"});
    AppParam::registerParam("cmake", {"cmake"});
    AppParam::registerParam("install", {"install"});

    AppParam::initialize(argc, argv);


    if(AppParam::has("install")) InstallCommand().execute();
    else {
        if(AppParam::has("cmake")){
            if(AppParam::has("build")){
                CmakeBuildCommand().execute();
            } else CMakeInitalizeCommand().execute();
        } else {
            if (BuildCommand().execute() == CommandResult::DependencyMissing) {
                printf("Use borac install.\n");
            }
        }
    }

    return 0;
}
