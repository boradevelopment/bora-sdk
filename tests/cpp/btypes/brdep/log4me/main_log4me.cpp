// Main File that is dependent on the log4me library without including the compilation of the log4me.cpp file.
#include <macros>
#include "log4me.h"
#include <iostream>

int main(){
    printf("Main regular printfs2 - nothing special\n");
    int i = 0;
    i++;
    std::cout << i << std::endl;
    log_print("somewhere else");
    return 0;
}