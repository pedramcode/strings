cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)
project(strings LANGUAGES C VERSION 0.0.1)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
