project(weavefra-lib VERSION 1.0.0)

include_directories(${PROJECT_NAME} PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/weavefra-lib/src
)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "src/*.c" "scr/*.h" "src/*.hpp")

add_library(${PROJECT_NAME} ${LIB_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)


if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _BUILDING_DLL)
endif()


target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:LIB_DEBUG=1>
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
