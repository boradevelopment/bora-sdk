// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include "BuildCommand.h"
#include "V2Archive.h"
#include <filesystem>
#include <ArchiveExport.h>

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
    std::filesystem::path emBaseFolder(exePath.parent_path().string()+R"(/.deps/emsdk/upstream/emscripten/)");
#endif
    auto sdkPath = Environment::getEnvVar("BORA_SDK_PATH");

    if(sdkPath.empty()){
        printf("SDK can not be found, bora sdk was not installed properly");
        return CommandResult::DependencyMissing;
    }

    if(!exists(emBaseFolder)) return CommandResult::DependencyMissing;
    auto asParam = AppParam::get("as");
    if(asParam.empty()) asParam = L"app";
    auto displayName = AppParam::get("displayName");
    auto output = AppParam::get("output");
    if(output.empty()){
        printf("I need a output path\n");
        return CommandResult::InvalidArguments;
    }

    if(displayName.empty()){
        displayName = std::filesystem::path(output).filename().stem().wstring();

    }

    auto logo = AppParam::get("logo");
    if(logo.empty()){
        logo = LR"(./borat.png)";
    }

    auto isDebug = AppParam::has("debug");

    auto inputFile = AppParam::get("");
    if(inputFile.empty()) return CommandResult::InvalidArguments;


    if(asParam == L"app"){

        std::filesystem::path outputPath(output+L".bapp");

        auto bwasmPath = std::filesystem::path(outputPath.filename().stem().string()+".wasm");
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args("-Oz -s STANDALONE_WASM -I");
        args.append("\""+sdkPath + "/include\" ");
        args.append("-o" + bwasmPath.string());
        args.append(" "+wstringToUtf8(inputFile));
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

    {

        V2Archive archive(outputPath.filename().wstring(), outputPath.string(), L"This is a BORA Application! You shouldn't touch anything unless you know what you're doing...");
        archive.header.customVariables[L"id"] = "BORA";
        archive.header.customVariables[L"entry"] = bwasmPath.filename().stem().wstring();
        archive.header.customVariables[L"displayName"] = displayName;
        archive.addFileAndGet(bwasmPath.wstring(), bwasmPath.filename().stem().wstring());
        archive.addFileAndGet(logo, L"logo");
        archive.header.logV2Header();
    }

        V2Archive archive;
        archive.output = outputPath.string();
        archive.getArchive();

        exportArchive(archive);

     printf("Your BORA application is complete! You can find it at %s\n", std::filesystem::absolute(outputPath).string().c_str());

    } else if(asParam == L"cdep"){
        auto bwasmPath = std::filesystem::path(exePath.parent_path().string() +"\\bwasm.o");
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args("-Oz -c -I");
        args.append("\""+sdkPath + "\\include\" ");
        args.append("-o" + bwasmPath.string());
        args.append(" "+wstringToUtf8(inputFile));
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

            int buildArchive = Command(cmdApp, {{cmdCode, emBaseFolder.string() + "emar rcs lib"+wstringToUtf8(output)+".bcdep " + bwasmPath.string()}}).execute([](auto output){
                printf("%s\n", output.c_str());
            });

            std::filesystem::remove(bwasmPath);

            if(buildArchive != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
            }

             printf("Your BORA Compile Dependency is complete! You can find it in %s", std::filesystem::absolute(L"lib"+output+L".bcdep").string().c_str());

     } else if(asParam == L"rdep") { // Runtime Dependency
        std::filesystem::path outputPath(output+L".brdep");

        auto bwasmPath = std::filesystem::path(outputPath.filename().stem().string()+".wasm");
        // RT Dependents are similiar to bapp, there is no entry point though.
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args("-Oz -s STANDALONE_WASM --no-entry -I");
        args.append("\""+sdkPath + "\\include\" ");
        args.append("-o" + bwasmPath.string());
        args.append(" "+wstringToUtf8(inputFile));
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


            {

                V2Archive archive(outputPath.filename().wstring(), outputPath.string(), L"This is a BORA Runtime Dependency that BORA applications may require.");
                archive.header.customVariables[L"id"] = "BORADEP";
                archive.header.customVariables[L"depname"] = output;
                archive.addFile(bwasmPath.wstring());
            }

          // std::filesystem::remove(bwasmPath);

            printf("Your BORA Runtime Dependency is complete! You can find it at %s\n", std::filesystem::absolute(outputPath).string().c_str());

    }

    return CommandResult::Success;
}