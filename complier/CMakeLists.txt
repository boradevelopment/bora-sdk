cmake_minimum_required(VERSION 3.14)
project(BORAC C CXX ASM)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE BORA_SOURCES_CONTRIB contribs/*.cpp contribs/*.h contribs/*.hpp ../../global/cpp/contribs/*.h ../../global/cpp/contribs/*.cpp ../../global/cpp/contribs/*.hpp)

include_directories(
        3rdparty
        ../../global/cpp/contribs/TAZA/code
       ../../global/cpp/contribs
)

# Collect source files
file(GLOB_RECURSE BORA_SOURCES src/*.cpp src/*.h src/*.hpp)

file(GLOB_RECURSE BORA_SOURCES_3RDPARTY 3rdparty/**/*.cpp 3rdparty/**/*.h 3rdparty/**/*.hpp)

# Add executable
add_executable(BORAC ${BORA_SOURCES}
        ${BORA_SOURCES_CONTRIB}
        ${BORA_SOURCES_3RDPARTY})

target_compile_options(BORAC PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-include${CMAKE_CURRENT_SOURCE_DIR}/../../global/cpp/contribs/TypeDefinitions.h>
        $<$<CXX_COMPILER_ID:GNU>:-include${CMAKE_CURRENT_SOURCE_DIR}/../../global/cpp/contribs/TypeDefinitions.h>
        $<$<CXX_COMPILER_ID:MSVC>:/FI${CMAKE_CURRENT_SOURCE_DIR}/../../global/cpp/contribs/TypeDefinitions.h>
)

target_compile_definitions(BORAC PRIVATE
        TAZABASEDIR=\"${CMAKE_SOURCE_DIR}../../global/cpp/contribs/TAZA/code\"
)

if (WIN32)
    file(GLOB_RECURSE BORA_SOURCES_3RDPARTYDLIB ../../global/libs/*.dlib)
target_link_libraries(BORAC PRIVATE ${BORA_SOURCES_3RDPARTYDLIB})
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   # file(GLOB_RECURSE BORA_SOURCES_3RDPARTYDLIB ../../global/libs/*.a)
    target_link_directories(BORAC PRIVATE ../../global/libs/__linux__)

    target_link_libraries(BORAC PRIVATE
            #${BORA_SOURCES_3RDPARTYDLIB}
            -lbrotlienc -lbrotlidec -lbrotlicommon
    )
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-4")
    set(CMAKE_OSX_ARCHITECTURES arm64)


    target_link_directories(BORAC PRIVATE ../../global/libs/__apple__)

    target_link_libraries(BORAC PRIVATE
            #${BORA_SOURCES_3RDPARTYDLIB}
            -lbrotlienc -lbrotlidec -lbrotlicommon
    )
endif ()