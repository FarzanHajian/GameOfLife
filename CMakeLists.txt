cmake_minimum_required(VERSION 3.15)

project(GameOfLive
    VERSION 0.1
    DESCRIPTION "A simple Raylib program that implements the Conway’s Game of Life"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/raylib)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

include(cmake/format_source.cmake)
add_custom_target(format COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES})

add_custom_target(check_license
    COMMAND ${CMAKE_COMMAND}
        -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
        -P ${PROJECT_SOURCE_DIR}/cmake/check_license.cmake
)