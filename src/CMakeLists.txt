cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Assignment2 C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Add the source files
add_executable(assignment2.c "proj2(onlyInUbuntu/assignment2.c" "proj2(onlyInUbuntu/semaphore.h"
        "proj2(onlyInUbuntu/sharedMemory.h"
        "prevHomeworks/proj1(onlyInUbuntu/assignment1.c")

# Link the pthread library
