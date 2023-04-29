cmake_minimum_required(VERSION 3.15.0)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sudoku VERSION 0.1.0)

include_directories(src)

# include test directory
add_subdirectory(tests)

# mark all .cpp files to be compiled
set(SOURCES "main.cpp")
aux_source_directory(src SOURCES)

# add the executable
add_executable(sudoku ${SOURCES})