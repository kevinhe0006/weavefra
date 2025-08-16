project(weavefra-lib)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "src/*.c" "src/*.h" "src/*.hpp")
add_library(${PROJECT_NAME} ${LIB_SOURCES})

target_precompile_headers(${PROJECT_NAME} PUBLIC
    src/WFpch.h
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE
      WEAVEFRA_STATIC
  )
else()
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE
      _BUILDING_DLL
  )
endif()

message(STATUS "  BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message(STATUS "  WEAVEFRA_STATIC  = ${WEAVEFRA_STATIC}")


target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:LIB_DEBUG=1>
)

# Detect platform and set OS macros
# ---- OS macros ----
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _OS_WINDOWS=1)
elseif(ANDROID)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _OS_UNIX=1 _OS_ANDROID=1)
elseif(APPLE)
  if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _OS_APPLE=1 _OS_IOS=1 _OS_UNIX=1)
  else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE _OS_APPLE=1 _OS_MACOS=1 _OS_UNIX=1)
  endif()
elseif(UNIX)
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _OS_UNIX=1 _OS_LINUX=1)
  else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE _OS_UNIX=1)
  endif()
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE _OS_UNKNOWN=1)
endif()

# ---- Arch width ----
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _ARCH_64BIT=1)
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE _ARCH_32BIT=1)
endif()

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
