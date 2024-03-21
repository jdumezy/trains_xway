cmake_minimum_required(VERSION 3.0.0)
project(trains_xway VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

include_directories(include)

add_library(xway_pkg src/xway_pkg.c)
add_library(xway_tcp src/xway_tcp.c)

add_executable(train src/train.c)

target_link_libraries(xway_tcp xway_pkg)
target_link_libraries(train xway_pkg xway_tcp Threads::Threads)
