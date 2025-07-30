// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

/* Common

 * FileName: Command.h
 * Title: ?
 * Author: ?
 * Purpose: ?

 * Compatibility: ?

 * Updates - ?
 * Known issues - ?
 */
#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>

class Command {
public:
    Command(const std::string& exe, const std::map<std::string, std::string>& arguments = {{"", ""}});
    int execute(std::function<void(const std::string&)> onOutputLine = nullptr);
    std::string output() const;

private:
    std::string executable;
    std::map<std::string, std::string> args;
    std::string outputStr;
    int resultCode;

    std::vector<std::string> buildArgsList();

#if defined(_WIN32)
    int executeWindows(std::function<void(const std::string&)>& onOutputLine);
#else
    int executePosix(std::function<void(const std::string&)>& onOutputLine);
#endif
};


