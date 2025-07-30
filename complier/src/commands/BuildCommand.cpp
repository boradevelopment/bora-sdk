// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include "BuildCommand.h"
#include "V2Archive.h"
#include <filesystem>
#if __linux__
#include <unistd.h>
#include <climits>
#endif

CommandResult BuildCommand::execute() {
    std::filesystem::path exePath;
    if(OsDependentPathGet(exePath) != CommandResult::Success){
        return CommandResult::Failure;
    }
#if __APPLE__
    std::filesystem::path emBaseFolder(exePath.string()+R"(/.deps/emsdk/upstream/emscripten/)");
#else
    std::filesystem::path emBaseFolder(exePath.parent_path().string()+R"(.deps/emsdk/upstream/emscripten/)");
#endif
    auto sdkPath = Environment::getEnvVar("BORA_SDK_PATH");

    if(sdkPath.empty()){
        printf("SDK can not be found, bora sdk was not installed properly");
        return CommandResult::DependencyMissing;
    }

    if(!exists(emBaseFolder)) return CommandResult::DependencyMissing;
    auto asParam = AppParam::getValue<std::string>("as");
    if(asParam.empty()) asParam = "app";
    auto output = AppParam::getValue<std::string>("output");
    if(output.empty()){
        printf("I need a output path\n");
        return CommandResult::InvalidArguments;
    }

    auto logo = AppParam::getValue<std::string>("logo");
    if(logo.empty()){
        logo = R"(./borat.png)";
    }

    auto isDebug = AppParam::has("debug");

    auto inputFile = AppParam::getValue<std::string>("");
    if(inputFile.empty()) return CommandResult::InvalidArguments;


    if(asParam == "app"){
        auto bwasmPath = std::filesystem::path(exePath.string() +"bwasm.wasm");
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args("-Oz -s STANDALONE_WASM -I");
        args.append("\""+sdkPath + "/include\" ");
        args.append("-o" + bwasmPath.string());
        args.append(" "+inputFile);
        args.append(" -Wl,--whole-archive \""+sdkPath + R"(/libs/libbora.bcdep" -Wl,--no-whole-archive)");

        // Build input file
        if(isDebug){
            args.append(" -g3");
            compPath.append(args);
        } else {
            compPath.append(args);
        }

        int bla = Command(cmdApp, {{cmdCode, compPath}}).execute([](auto output){
            printf("%s\n", output.c_str());
        });



        if(bla != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
        }

    std::filesystem::path outputPath(output+".bapp");

    {

        V2Archive archive(outputPath.filename().string(), outputPath.string(), "This is a BORA Application! You shouldn't touch anything unless you know what you're doing...");
        archive.header.customVariables["id"] = "BORA";
        archive.header.customVariables["entry"] = bwasmPath.filename().string();
        archive.addFileAndGet(bwasmPath.string(), bwasmPath.filename().string());
        archive.addFileAndGet(logo, "logo");
    }

     printf("Your BORA application is complete! You can find it at %s\n", std::filesystem::absolute(outputPath).string().c_str());

    } else if(asParam == "cdep"){
        auto bwasmPath = std::filesystem::path(exePath.parent_path().string() +"\\bwasm.o");
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args("-Oz -c -I");
        args.append("\""+sdkPath + "\\include\" ");
        args.append("-o" + bwasmPath.string());
        args.append(" "+inputFile);
        if(!AppParam::has("nobora")) args.append(" \""+sdkPath + "\\libs\\libbora.bcdep");

        if(isDebug){
            args.append(" -g3");
            compPath.append(args);
        } else {
            compPath.append(args);
        }

            int buildObject = Command(cmdApp, {{cmdCode, compPath}}).execute([](auto output){
                printf("%s\n", output.c_str());
            });

            if(buildObject != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
            }

            int buildArchive = Command(cmdApp, {{cmdCode, emBaseFolder.string() + "emar rcs lib"+output+".bcdep " + bwasmPath.string()}}).execute([](auto output){
                printf("%s\n", output.c_str());
            });

            std::filesystem::remove(bwasmPath);

            if(buildArchive != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
            }

             printf("Your BORA Compile Dependency is complete! You can find it in %s", std::filesystem::absolute("lib"+output+".bcdep").string().c_str());

     } else if(asParam == "rdep") { // Runtime Dependency
        auto bwasmPath = std::filesystem::path(exePath.parent_path().string() +"bwasm.wasm");
        // RT Dependents are similiar to bapp, there is no entry point though.
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args("-Oz -s STANDALONE_WASM --no-entry -I");
        args.append("\""+sdkPath + "\\include\" ");
        args.append("-o" + bwasmPath.string());
        args.append(" "+inputFile);
        args.append(" -Wl,--whole-archive \\\"\"+sdkPath + R\"(\\libs\\libbora.bcdep\" -Wl,--no-whole-archive)\"");
        compPath.append(args);

        if(isDebug){
            args.append(" -g3");
            compPath.append(args);
        } else {
            compPath.append(args);
        }

            int bla = Command(cmdApp, {{cmdCode, compPath}}).execute([](auto output){
                printf("%s\n", output.c_str());
            });

            if(bla != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
            }

            std::filesystem::path outputPath(output+".brdep");

            {

                V2Archive archive(outputPath.filename().string(), outputPath.string(), "This is a BORA Runtime Dependency that BORA applications may require.");
                archive.header.customVariables["id"] = "BORADEP";
                archive.header.customVariables["depname"] = output;
                archive.addFile(bwasmPath.string());
            }

           std::filesystem::remove(bwasmPath);

            printf("Your BORA Runtime Dependency is complete! You can find it at %s\n", std::filesystem::absolute(outputPath).string().c_str());

    }

    return CommandResult::Success;
}