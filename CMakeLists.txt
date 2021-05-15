cmake_minimum_required(VERSION 2.8.12)
project(norbit)

include_directories(include)

add_definitions(-std=c++17
                -Wall
                -Werror
                -g
                )

add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
