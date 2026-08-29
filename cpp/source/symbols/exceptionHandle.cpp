#include "symbols/exceptionHandle.h"
#include <iostream>
#include <exception>

static inline ExceptionCallback callback_ = nullptr;

void ExceptionHandler::setCallback(ExceptionCallback cb) {
        callback_ = cb;
}

ExceptionCallback ExceptionHandler::getCallback()
{
    return callback_;
}

int ExceptionHandler::triggerException(__cxa_exception* tObject, const std::string &message) {
    std::cout << "Triggering Exception | " << message << std::endl;
        if (callback_) {
            return callback_(tObject, message);
        } else {
            return 0;
        }
}

extFunc {
    int bora_exception_handler(i64 tObject, i64 exceptionDetails){
        const char* details = (const char*)exceptionDetails;
        __cxa_exception* exceptionHeader =
        static_cast<__cxa_exception*>((void*)tObject) - 1;
        return ExceptionHandler::triggerException(exceptionHeader, details);
    }
}