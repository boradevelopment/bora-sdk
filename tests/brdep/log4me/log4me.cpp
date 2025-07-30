#include "log4me.h"

extFunc {
void log_print(const char* data){
    bora::logging::print("Based off log4me runtime dependent!");
    bora::logging::print(data);
}
}
