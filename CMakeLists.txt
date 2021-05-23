cmake_minimum_required(VERSION 3.1)
project(compres C)

set(CMAKE_C_STANDARD 99)

add_executable(compres compres.c)
add_executable(decompres decompres.c)

install(TARGETS compres decompres
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

add_custom_target(clean-all
  make clean && rm -fR ${CMAKE_BINARY_DIR}/CMakeCache.txt
                       ${CMAKE_BINARY_DIR}/cmake_install.cmake
                       ${CMAKE_BINARY_DIR}/Makefile
                       ${CMAKE_BINARY_DIR}/CMakeFiles
                       ${CMAKE_BINARY_DIR}/install_manifest.txt
)
