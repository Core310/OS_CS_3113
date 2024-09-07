cmake_minimum_required(VERSION 3.29)
project(CS3203 C)

set(CMAKE_C_STANDARD 23)

add_executable(CMAKE_C_STANDARD
        main.c
        hw1/Hw1.c
        hw1/Hw1.h
        genUtilsLib/arika_utils.c
        genUtilsLib/arika_utils.h)
