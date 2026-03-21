// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include "BuildCommand.h"
#include "V2Archive.h"
#include <filesystem>
#include <ArchiveExport.h>
#include <codecvt>

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
    auto includeDirectories = AppParam::getArray("include");
    auto libBoraDevelopment = AppParam::has("useRootSource");
    bool useProductionBin = std::filesystem::exists(sdkPath+"/cpp/bin");
    if (!useProductionBin) {
        printf("BIN not found! This is irregular to happen for a end user so assuming this is a developer setup!\n");
    }
    if (useProductionBin && libBoraDevelopment) {
        useProductionBin = false;
    }

    auto devRootPath = Environment::getEnvVar("BORA_DEV_ROOT_PATH");
    if(!devRootPath.empty() && !useProductionBin) {
        printf("BORA DEVELOPER SOURCE DETECTED - Including global contributions folder for development purposes! Note that by default, contributions are not  included post production! Please write // BORA_INCLUDED_IN_SDK on the top of the contributing header in order for it to be included in production builds.\n");
        includeDirectories.push_back(std::filesystem::path(devRootPath) / "global/cpp/contribs");
    } else if (devRootPath.empty() && !useProductionBin) {
        printf("BORA DEVELOPER SOURCE NOT FOUND - Please run setup.bat at the root of your source directory (if you don't know what this means, remove -useRootSource)");
        return CommandResult::DependencyMissing;
    }

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
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1");
        std::string args(" -s STANDALONE_WASM -I");
        args.append("\""+sdkPath + "/include\" -I \""+sdkPath + "/bin/include\"");
        for (const auto& path : includeDirectories) {
            args.append(" -I\"");
            args.append(wstringToUtf8(path));
            args.append("\" ");
        }
        args.append(" -o " + bwasmPath.string());
        args.append(" "+wstringToUtf8(inputFile));
        args.append(" -Wl,--whole-archive \""+sdkPath + R"(/libs/libbora.bcdep" -Wl,--no-whole-archive)");

        // Build input file
        if(isDebug){
            args.append(" -g3 -O0 -mno-reference-types -mno-sign-ext -s SAFE_HEAP=1 -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1");
            compPath.append(args);
        } else {
            args.append(" -Oz -mno-reference-types -mno-sign-ext -s ALLOW_MEMORY_GROWTH=1 -s SAFE_HEAP_LOG=1");
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
        std::filesystem::path outputPath(output);
        auto bwasmPath = std::filesystem::path(outputPath.parent_path().string() +"\\bwasm.o");
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args("-sSTANDALONE_WASM=1 -c -I");
        args.append("\""+sdkPath + "\\include\" ");
        for (const auto& path : includeDirectories) {
            args.append("-I\"");
            args.append(wstringToUtf8(path));
            args.append("\" ");
        }
        args.append("-o" + bwasmPath.string());
        args.append(" "+wstringToUtf8(inputFile));
        if(!AppParam::has("nobora")) args.append(" \""+sdkPath + "\\libs\\libbora.bcdep");

        if(isDebug){
            args.append("  -O0 -g4 -s ASSERTIONS=1");
            compPath.append(args);
        } else {
            args.append(" -Oz");
            compPath.append(args);
        }

            int buildObject = Command(cmdApp, {{cmdCode, compPath}}).execute([](auto output){
                printf("%s\n", output.c_str());
            });

            if(buildObject != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
            }

            outputPath.replace_filename(L"lib"+outputPath.filename().generic_wstring()+L".bcdep");
            int buildArchive = Command(cmdApp, {{cmdCode, emBaseFolder.string() + "emar rcs " + outputPath.generic_string() + bwasmPath.string()}}).execute([](auto output){
                printf("%s\n", output.c_str());
            });

            std::filesystem::remove(bwasmPath);

            if(buildArchive != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
            }

             printf("Your BORA Compile Dependency is complete! You can find it in %s", outputPath.string().c_str());

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