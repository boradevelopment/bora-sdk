cmake_minimum_required(VERSION 3.14)
project(BORAC C CXX ASM)
set(CMAKE_CXX_STANDARD 17)

# Collect source files
file(GLOB_RECURSE BORA_SOURCES src/*.cpp src/*.h src/*.hpp)

# Add executable
add_executable(BORAC ${BORA_SOURCES})
