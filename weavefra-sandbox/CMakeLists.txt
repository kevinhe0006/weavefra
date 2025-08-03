project(weavefra-sandbox LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME}
PUBLIC
${Weavefra_Lib_Dir}
${CMAKE_CURRENT_SOURCE_DIR}/weavefra-sandbox/src
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
weavefra-lib
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:APP_DEBUG=1>
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)