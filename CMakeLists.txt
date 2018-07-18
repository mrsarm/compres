cmake_minimum_required(VERSION 3.10)
project(decompres C)

set(CMAKE_C_STANDARD 99)

add_executable(compres compres.c)
add_executable(decompres decompres.c)
