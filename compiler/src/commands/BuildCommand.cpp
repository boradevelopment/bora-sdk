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

REGISTER_PARAM("id")
REGISTER_PARAM_AD("define", {"-D"}, "Preprocessor macro definitions");
REGISTER_PARAM_AD("exportall", {"-ea"}, "Preprocessor macro definitions");
REGISTER_PARAM_AD("mainmodule", {"-mm"}, "Preprocessor macro definitions");

CommandResult BuildCommand::execute() {
    std::filesystem::path exePath;
    if(OsDependentPathGet(exePath) != CommandResult::Success){
        return CommandResult::Failure;
    }
    std::filesystem::path emBaseFolder(exePath.parent_path().string()+R"(/.deps/emsdk/upstream/emscripten/)");
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
    auto binaryID = AppParam::get("id");
    auto includeDirectories = AppParam::getArray("include");
    auto libBoraDevelopment = AppParam::has("useRootSource");
    bool useProductionBin = std::filesystem::exists(sdkPath+"/bin");
    if (!useProductionBin) {
        printf("BIN not found! This is irregular to happen for a end user so assuming this is a developer setup!\n");
    }
    if (useProductionBin && libBoraDevelopment) {
        useProductionBin = false;
    }

    auto devRootPath = Environment::getEnvVar("BORA_DEV_ROOT_PATH");
    if(!devRootPath.empty() && !useProductionBin) {
        printf("BORA DEVELOPER SOURCE DETECTED - Including global contributions folder for development purposes! Note that by default, contributions are not  included post production! Please write // BORA_INCLUDED_IN_SDK on the top of the contributing header in order for it to be included in production builds.\n");
        includeDirectories.push_back((std::filesystem::path(devRootPath) / L"global/cpp/contribs").wstring());
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

    if(asParam == L"app" || asParam == L"rdep"){
        bool isApp = asParam == L"app";
        std::filesystem::path outputPath;
        if(isApp) {
           outputPath = std::filesystem::path(output+L".bapp");
        } else {
            outputPath = std::filesystem::path(output+L".brdep");
        }
        std::filesystem::path bwasmPath = std::filesystem::path(outputPath.filename().stem().string()+".wasm");

        if (binaryID.empty())
        {
            binaryID = outputPath.filename().stem().wstring();
        }
        std::string compPath(emBaseFolder.string() + "em++ -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s MEMORY64=1 ");
        std::string args("");
         if(!AppParam::has("nobora")) args.append("-I \""+sdkPath + "/include\" -I \""+sdkPath + "/bin/include\"");
        for (const auto& path : includeDirectories) {
            args.append(" -I\"");
            args.append(wstringToUtf8(path));
            args.append("\" ");
        }
        args.append("  -o " + bwasmPath.string());
        args.append(" "+wstringToUtf8(inputFile));
        args.append(" -Wno-deprecated -Wdeprecated-declarations -Wl,--stack-first -s STACK_SIZE=1MB -Wl,--export=__wasm_apply_relocs -Wl,--export=__wasm_apply_global_relocs  -Wl,--export-memory -s STANDALONE_WASM -s RELOCATABLE=1 -fvisibility=hidden -fvisibility-inlines-hidden -s IMPORTED_MEMORY=1 -Wl,--import-memory -Wl,--gc-sections -s MAXIMUM_MEMORY=16gb -s ALLOW_MEMORY_GROWTH=1 ");
            if(!AppParam::has("nobora")) args.append("  -Wl,--export=__call_dtors -Wl,--export=get_bora_sdk_version -Wl,--export=get_bora_sdk_version_major -Wl,--export=get_bora_sdk_version_minor -Wl,--export=get_bora_sdk_mode -Wl,--no-whole-archive \""+sdkPath + R"(/libs/libbora.bcdep")");

        args.append(" -D__BORA__ID=\""+wstringToUtf8(binaryID)+"\"");
        if (isApp)
        {
            args.append(" -D__BORA__APPLICATION");
        } else
        {
            args.append(" --no-entry -D__BORA__SHARED");
        }
        if (AppParam::has("exportall"))
        {
            args.append(" -s EXPORT_ALL=1");
        }

        std::vector<std::wstring> rawDefs = AppParam::getValues("define");
        for (const auto& definition : rawDefs)
        {
            args.append(" -D"+wstringToUtf8(definition));
        }

        // Build input file
        if(isDebug){ 
            args.append(" -g3 -O0");
            compPath.append(args);
        } else {
            args.append(" -Oz");
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
        V2Archive archive(outputPath.filename().wstring(), outputPath.string(), L"This is a BORA Binary File! You shouldn't touch anything unless you know what you're doing...");
        if (isDebug) archive.nocompression = true;
        archive.header.customVariables[L"id"] = binaryID;
        archive.header.customVariables[L"entry"] = bwasmPath.filename().stem().wstring();
        if (isApp) {
            archive.header.customVariables[L"magic"] = "BORA";
            archive.header.customVariables[L"type"] = "APP";
            archive.header.customVariables[L"displayName"] = displayName;
        } else {
            archive.header.customVariables[L"magic"] = "BORA";
            archive.header.customVariables[L"type"] = "DEP";
        }

        archive.addFileAndGet(bwasmPath.wstring(), bwasmPath.filename().stem().wstring());
        archive.addFileAndGet(logo, L"logo");

        if (AppParam::has("logfile"))
        { // todo: output path
            archive.header.logV2Header();
        }

        }

        // removals
        // std::filesystem::remove(bwasmPath);
        if (isApp) {
            printf("Your BORA application is complete! You can find it at %s\n", std::filesystem::absolute(outputPath).string().c_str());
        } else {
            printf("Your BORA Runtime Dependency is complete! You can find it at %s\n", std::filesystem::absolute(outputPath).string().c_str());
        }
    } else if(asParam == L"cdep"){
        std::filesystem::path outputPath(output);
        auto bwasmPath = std::filesystem::path(outputPath.parent_path().string() +"/bwasm.o");
        std::string compPath(emBaseFolder.string() + "em++ -s MEMORY64=1 ");
        std::string args(" -s RELOCATABLE=1  -Wno-deprecated -Wdeprecated-declarations -fPIC -c -I");
        args.append("\""+sdkPath + "/include\" ");
        for (const auto& path : includeDirectories) {
            args.append("-I\"");
            args.append(wstringToUtf8(path));
            args.append("\" ");
        }
        args.append("-fvisibility=hidden -fvisibility-inlines-hidden -o" + bwasmPath.string());
        args.append(" "+wstringToUtf8(inputFile));
        if(!AppParam::has("nobora")) args.append(" \""+sdkPath + "\\libs\\libbora.bcdep");

        args.append(" -D__BORA__STATIC");

        if(isDebug){
            args.append("  -O0 -g");
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

            if(buildArchive != 0){
                printf("Please fix the following errors ^^^\n");
                return CommandResult::Failure;
            }

             std::filesystem::remove(bwasmPath);

            printf("Your BORA Compile Dependency is complete! You can find it in %s", outputPath.string().c_str());

     }

    return CommandResult::Success;
}