##
##  CMakeLists.txt
##  STUNExternalIP
##
##  Created by FireWolf on 2025-12-10.
##

cmake_minimum_required(VERSION 3.20)

# Project Definition
project("STUNExternalIP")

# Set the language standard to `c11`
set(CMAKE_C_STANDARD 11)

# Set the compiler flags for GCC and Clang compilers
set(CMAKE_C_FLAGS                   "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE}")

# Target: Playground
set(TARGET              ${PROJECT_NAME})
add_executable(${TARGET} "main.c" "STUNExternalIP.c")
