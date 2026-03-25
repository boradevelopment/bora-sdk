// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

#include "Command.h"

#include <vector>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <array>
#include <signal.h>

#if defined(_WIN32)
#include <windows.h>
#include <iostream>

#else
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#endif

Command::Command(const std::string& exe, const std::map<std::string, std::string>& arguments)
        : executable(exe), args(arguments), resultCode(-1) {}

std::string Command::output() const {
    return outputStr;
}

int Command::getResult() const {
    return resultCode;
}

void Command::kill() {
#if WIN32
    if (pi.hProcess) {  // pi is your PROCESS_INFORMATION member
        // Forcefully terminate the process
        TerminateProcess(pi.hProcess, 1); // 1 is the exit code

        // Wait for the process to actually exit
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        pi.hProcess = nullptr;
        pi.hThread = nullptr;
    }
#elif defined(__linux__) || defined(__APPLE__)
    if (pid > 0) {
        // Send SIGTERM first (polite termination)
        ::kill(pid, SIGTERM);

        // Wait a short time for process to exit
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == 0) {
            // Process still alive, force kill with SIGKILL
            ::kill(pid, SIGKILL);
            waitpid(pid, &status, 0); // wait for it to exit
        }

        pid = 0; // mark as no longer active
    }
#endif
}

int Command::execute(std::function<void(const std::string&)> onOutputLine) {
#if defined(_WIN32)
    return executeWindows(onOutputLine);
#else
    return executePosix(onOutputLine);
#endif
}

std::vector<std::string> Command::buildArgsList() {
    std::vector<std::string> result;
    result.push_back(executable);
    for (const auto& [key, val] : args) {
        if (!key.empty()) result.push_back(key);
        if (!val.empty()) result.push_back(val);
    }
    return result;
}

#if defined(_WIN32)
int Command::executeWindows(std::function<void(const std::string&)>& onOutputLine) {
    std::vector<std::string> argList = buildArgsList();
    std::ostringstream oss;
    for (const std::string& arg : argList) {
        if(argList[0] == arg){
            oss << arg;
        } else if(arg.find(' ') == std::string::npos){
            oss << " " << arg;
        } else oss << "\"" << arg << "\" ";
    }
        //oss << arg
    std::string cmdLine = oss.str();

    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa{ sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return -1;

    STARTUPINFOA si{};
    si.cb = sizeof(STARTUPINFOA);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;

    BOOL success = CreateProcessA(
            nullptr, (LPSTR)cmdLine.c_str(), nullptr, nullptr, TRUE,
            0, nullptr, nullptr, &si, &pi
    );

    CloseHandle(hWrite); // parent reads only

    if (!success) {
        DWORD errorCode = GetLastError();

        LPVOID lpMsgBuf;
        FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf,
                0, nullptr);

        std::wcerr << L"Win32 API failed with error: " << (LPWSTR)lpMsgBuf << std::endl;
        LocalFree(lpMsgBuf);

        CloseHandle(hRead);
        return -1;
    }

    std::string line;
    std::array<char, 128> buffer;
    DWORD bytesRead;
    while (ReadFile(hRead, buffer.data(), buffer.size(), &bytesRead, nullptr)) {
        if (bytesRead == 0) break;

        for (DWORD i = 0; i < bytesRead; ++i) {
            char c = buffer[i];

            if (!onOutputLine) {
                // Direct passthrough to stdout if no callback
                putchar(c);
                fflush(stdout);
                continue;
            }

            // Handle newlines
            if (c == '\n') {
                onOutputLine(line);
                outputStr += line + "\n";
                line.clear();
            }
            // Handle carriage return (progress updates like curl)
            else if (c == '\r') {
                // Treat it as a complete update (overwrite same line)
                if (!line.empty()) {
                    onOutputLine(line);
                    outputStr += line + "\n";
                    line.clear();
                }
            }
            else {
                line += c;
            }
        }

        // Flush any partial data (for real-time feedback)
        if (!line.empty()) {
            onOutputLine(line);
            outputStr += line;
            line.clear();
        }
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    resultCode = static_cast<int>(exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);

    return resultCode;
}
#else
int Command::executePosix(std::function<void(const std::string&)>& onOutputLine) {
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;

    pid = fork();
    if (pid == -1) return -1;

    if (pid == 0) {
        // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        std::vector<std::string> argList = buildArgsList();
        std::vector<char*> argv;
        for (auto& arg : argList)
            argv.push_back(const_cast<char*>(arg.c_str()));
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        _exit(127);
    }

    // Parent process
    close(pipefd[1]);
    char buffer[128];
    std::string line;
    ssize_t count;
    while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < count; ++i) {
            char c = buffer[i];
            if (c == '\n') {
                if (onOutputLine) onOutputLine(line);
                outputStr += line + "\n";
                line.clear();
            } else if (c != '\r') {
                line += c;
            }
        }
    }
    if (!line.empty()) {
        if (onOutputLine) onOutputLine(line);
        outputStr += line + "\n";
    }

    close(pipefd[0]);

    int status;
    waitpid(pid, &status, 0);
    resultCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    return resultCode;
}
#endif
