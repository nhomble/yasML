cmake_minimum_required(VERSION 3.12)
project(yasML C)

set(CMAKE_C_STANDARD 11)

add_library(matrix_lib matrices.h matrices.c)
add_executable(tests test.c)

target_link_libraries(tests matrix_lib)
