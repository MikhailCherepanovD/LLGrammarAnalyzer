cmake_minimum_required(VERSION 3.27)
project(Lab5Grammar)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab5Grammar main.cpp
        Generator.cpp
        Grammar.h)
