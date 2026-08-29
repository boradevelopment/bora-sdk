#pragma once

#include <functional>
#include <string>
#include <macros>
#include <cxxabi.h>


namespace __cxxabiv1 {
struct  __cxa_exception {
  size_t referenceCount;
  std::type_info *exceptionType;
  // In wasm, destructors return 'this' as in ARM
  void* (*exceptionDestructor)(void *);
  uint8_t caught;
  uint8_t rethrown;
  void *adjustedPtr;
  // Add padding to ensure that the size of __cxa_exception is a multiple of
  // the maximum useful alignment for the target machine.  This ensures that
  // the thrown object that follows has that correct alignment.
  void *padding;
};
}

using namespace abi;
typedef __cxa_exception cppException;

using ExceptionCallback = std::function<int(cppException* exception, const std::string &message)>;

class ExceptionHandler {
public:
    // Set the callback
   static void setCallback(ExceptionCallback cb);

   static ExceptionCallback getCallback();

    // Trigger an exception (called by runtime)
   static int triggerException(cppException* exception, const std::string &message);
};

 extFunc {
    // Bora supports exception handling for when an exception has been set!
    // When a exception is called, two types of metadata will be send to the handler
    // The thrown object and the details
    // The thrown object is a castable objec that can be casted to as a __cxa_exception for developer analysis
    // Whiled details are just characters for when the thrown object simply doesn't exist or cannot be given.
    EXPORT_ATTR("bora_exception_handler")
    int bora_exception_handler(i64 tObject, i64 details);
 }