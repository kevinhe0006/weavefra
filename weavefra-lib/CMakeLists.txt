project(weavefra-lib VERSION 1.0.0)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "src/*.c" "src/*.h" "src/*.hpp")
add_library(${PROJECT_NAME} ${LIB_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    ${CMAKE_DL_LIBS}
)

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

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
