// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

/* ? Common

 * FileName: CommandTemplate.h
 * Title: Command Template
 * Author: Munashe Dirwayi
 * Purpose: Serves as a template for compilier commands [Not to be confused with tools/Command, those are for commandline execution]

 * Compatibility: ?

 * Updates - ?
 * Known issues - ?
 */

#ifndef BORAC_COMMANDTEMPLATE_H
#define BORAC_COMMANDTEMPLATE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../tools/AppParam.h"
#include "../tools/Command.h"
#if WIN32
#include <Windows.h>
#endif
#include "../tools/Env.h"
#if __APPLE__
#include <mach-o/dyld.h>
#endif
#if __linux__
#include <unistd.h>
#include <climits>
#endif

enum class CommandResult {
        Success,
        Failure,
        InvalidArguments,
        NotImplemented,
        RuntimeError,
        PermissionDenied,
        DependencyMissing
};

#if defined(_WIN32)
static constexpr const char* cmdApp = "cmd.exe";
static constexpr const char* cmdCode = "/C";
#elif defined(__linux__) || defined(__APPLE__)
static constexpr const char* cmdApp = "sh";
static constexpr const char* cmdCode = "-c";
#else
static constexpr const char* cmdApp = "";
static constexpr const char* cmdCode = "";
#endif

static CommandResult OsDependentPathGet(std::filesystem::path& exePath);

CommandResult OsDependentPathGet(std::filesystem::path &exePath) {
#if WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    exePath = std::filesystem::path(buffer);
#elif __linux__
    char buffer[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (count == -1) {
        // Handle error appropriately
        return CommandResult::Failure;
    }
    exePath = std::filesystem::path (std::string(buffer, count));
#elif __APPLE__
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0) {
        // Buffer too small; handle error
        return CommandResult::Failure;
    }
    exePath = std::filesystem::canonical(buffer);

    exePath = exePath.parent_path();
#endif

    return CommandResult::Success;
}

class CommandTemplate {
    public:
        virtual ~CommandTemplate() = default;

        // Optional: command name (e.g., "install", "build", etc.)
        virtual std::string name() const = 0;

        // Executes the command with a parsed context
        virtual CommandResult execute() = 0;

        // Optional help/description
        virtual std::string description() const {
            return "No description provided.";
        }
    };

#endif //BORAC_COMMANDTEMPLATE_H
