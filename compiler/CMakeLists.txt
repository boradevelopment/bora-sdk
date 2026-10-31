cmake_minimum_required(VERSION 3.14)
project(BORAC C CXX)
set(CMAKE_CXX_STANDARD 20)
option(DEBUGGING "Enable Debugging features (pdb, etc)" OFF)
include(${CMAKE_SOURCE_DIR}/../../global/cmake/common.cmake)

file(GLOB_RECURSE BORA_SOURCES_CONTRIB contribs/*.cpp contribs/*.h contribs/*.hpp ../../global/cpp/contribs/*.h ../../global/cpp/contribs/*.cpp ../../global/cpp/contribs/*.hpp)
file(GLOB_RECURSE BORA_SOURCES src/*.cpp src/*.h src/*.hpp)
file(GLOB_RECURSE BORA_SOURCES_3RDPARTY 3rdparty/**/*.cpp 3rdparty/**/*.h 3rdparty/**/*.hpp ../../3rdparty/**/*.cpp ../../3rdparty/**/*.h ../../3rdparty/**/*.hpp)

include_directories(
        3rdparty
        ../../global/cpp/contribs/TAZA/code
       ../../global/cpp/contribs
        ../../global/cpp/3rdparty
)

# Collect source files
# Add executable
add_executable(BORAC ${BORA_SOURCES}
        ${BORA_SOURCES_CONTRIB}
        ${BORA_SOURCES_3RDPARTY}
     )


target_compile_definitions(BORAC PRIVATE
        NOMINMAX
        TAZABASEDIR=\"${CMAKE_SOURCE_DIR}../../global/cpp/contribs/TAZA/code\"
)




if (WIN32)
    file(GLOB_RECURSE BORA_SOURCES_3RDPARTYDLIB ../../global/libs/*.dlib)
      target_link_libraries(BORAC PRIVATE
              ${BORA_SOURCES_3RDPARTYDLIB})

elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(BORAC PRIVATE stdc++)
    target_link_directories(BORAC PRIVATE ../../global/libs/__linux__)
   target_compile_options(BORAC PRIVATE -include ${CMAKE_SOURCE_DIR}/../../global/cpp/contribs/TypeDefinitions.h)
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
endif()

set_target_properties(BORAC PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${SYSTEM_NAME_LOWER}/${ARCH}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${SYSTEM_NAME_LOWER}/${ARCH}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${SYSTEM_NAME_LOWER}/${ARCH}"
)
