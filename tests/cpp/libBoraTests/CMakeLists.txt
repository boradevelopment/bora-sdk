cmake_minimum_required(VERSION 3.13)

project(libboratests)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_executable(libboratests
    ${SOURCES}
)

foreach(item IN LISTS SOURCES)
    message(STATUS "Item: ${item}")
endforeach()

target_include_directories(libboratests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)


target_compile_options(libboratests PRIVATE -include TypeDefinitions.h)
