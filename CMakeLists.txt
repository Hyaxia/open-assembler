cmake_minimum_required(VERSION 3.23)
project(assembler C)

set(CMAKE_C_STANDARD 90)
add_compile_options(-Wall -pedantic)
file(GLOB all
        "*.h"
        "*.c"
        )
add_executable(assembler ${all})
