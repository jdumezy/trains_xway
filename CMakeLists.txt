cmake_minimum_required(VERSION 3.0.0)
project(trains_xway VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_definitions(DEBUG)
endif()

include_directories(include)

add_library(xway_msg src/xway_msg.c)

add_executable(main src/main.c)
add_target_library(main xway_msg)
