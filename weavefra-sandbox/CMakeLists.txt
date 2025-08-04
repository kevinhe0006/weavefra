project(weavefra-sandbox LANGUAGES CXX)

file(GLOB_RECURSE APP_RECURSE "src/*.cpp" "src/*.hpp" "src/*.h" "src/*.c")

add_executable(${PROJECT_NAME} ${APP_RECURSE})

target_include_directories(${PROJECT_NAME}
PUBLIC
${Weavefra_Lib_Dir}
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
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