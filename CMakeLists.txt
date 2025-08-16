cmake_minimum_required(VERSION 3.20.0)

project(weavefra
    VERSION 0.0.0
    DESCRIPTION "Simple C++ interface library"
    HOMEPAGE_URL "https://github.com/kevinhe0006/weavefra"
    LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(Weavefra_Lib_Dir ${CMAKE_SOURCE_DIR}/weavefra-lib/src)

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)
if(BUILD_TYPE_UPPER STREQUAL "DEBUG")
    add_compile_definitions(_DEBUG)
endif()

# Common macros for all
if(MSVC)
    add_compile_definitions(_COMPILER_MSVC)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_definitions(_COMPILER_CLANG)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(_COMPILER_GCC)
endif()

message(STATUS "Detected OS: ${OS_TYPE}")

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)
if(BUILD_TYPE_UPPER STREQUAL "DEBUG")
    add_compile_definitions(_DEBUG)
endif()

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(weavefra-lib)
add_subdirectory(weavefra-previewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


