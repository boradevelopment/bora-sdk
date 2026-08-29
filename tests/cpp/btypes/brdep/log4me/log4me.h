// Log4Me Library - Example Library of what you can create as a runtime dependency on BORA
// This is an example of a static runtime dependent library that you can produce on BORA.
#pragma once
#include <macros> // <- in order to use EXPORT_ATTR and extFunc though they can implemented by you no problem.   

#ifdef __BORA__SHARED
    #ifdef BUILDING_LOG4ME
        #define L4M_SHARED
    #endif
#endif

#if defined(L4M_STATIC) 
    #define LOG4ME_API(function) // For static libraries, attributes must be completely empty as they are treated as apart of the compiled application. So we wouldn't be exporting nor importing.
#elif defined(L4M_SHARED)
    #define LOG4ME_API(function) EXPORT_ATTR(function) // Building a shared library so we MUST export otherwise no one will be able to use our library!
#else
    #define LOG4ME_API(function) IMPORT_ATTR("log4me", function) // We're neither building a shared library or compiling as a app so we are a standard header library.
#endif

extFunc { // Export calls need to be formatted as external 'C' functions or extFunc.
// Importation Formatting - RuntimeID::Namespace::Function
// log4me$print -> log4me::log4me::print [log4me is both namespace and runtime ID]
// If i were to export it as print only -> log4me::print because there is no namespace.
LOG4ME_API("print") // Bora namespaces are formatted as $ rather than ::, there is no limit into how many namespaces you go specify.
void log_print(const char* data);
}