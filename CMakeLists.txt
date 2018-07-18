cmake_minimum_required(VERSION 3.9)
project(decompres C)

set(CMAKE_C_STANDARD 99)

add_executable(compres compres.c)
add_executable(decompres decompres.c)
