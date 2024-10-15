cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Assignment2 C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Add the source files
add_executable(assignment2.c "proj2onlyInUbuntu/assignment2.c" "proj2onlyInUbuntu/semaphore.h"
        "proj2onlyInUbuntu/sharedMemory.h"
        "prevHomeworks/proj1(onlyInUbuntu/assignment1.c"
        "CopySingleFileproj2onlyInUbuntu/assignment2copy.c"
        )

# Link the pthread library
