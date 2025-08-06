cmake_minimum_required(VERSION 3.20.0)

project(weavefra
    VERSION 0.0.0
    DESCRIPTION "Simple C++ interface library"
    HOMEPAGE_URL "https://github.com/kevinhe0006/weavefra"
    LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)


set(Weavefra_Lib_Dir ${CMAKE_SOURCE_DIR}/weavefra-lib/src)

set(GLFW_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/glfw)

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)
if(BUILD_TYPE_UPPER STREQUAL "DEBUG")
    add_compile_definitions(_DEBUG)
endif()
if(WIN32)
    add_compile_definitions(_WIN)
    if(BUILD_SHARED_LIBS)
        add_compile_definitions(_BUILDING_DLL)
    endif()
    set(OS_TYPE "Windows")
elseif(UNIX OR APPLE)
    add_compile_definitions(_UNIX)
    set(OS_TYPE "Unix-like")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)
if(BUILD_TYPE_UPPER STREQUAL "DEBUG")
    add_compile_definitions(_DEBUG)
endif()

if (UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
elseif(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(COREVIDEO_FRAMEWORK CoreVideo)
    find_library(OPENGLL_LIBRARY OpenGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${COREVIDEO_FRAMEWORK}
        ${OPENGLL_LIBRARY}
    )
elseif(WIN32)
    # GLFW's CMakeLists under Windows will automatically link Win32, user32, gdi32, opengl32, etc.
endif()

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/include
${GLFW_SOURCE_DIR}/include
)

add_subdirectory(${GLFW_SOURCE_DIR})
add_subdirectory(weavefra-lib)
add_subdirectory(weavefra-previewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


