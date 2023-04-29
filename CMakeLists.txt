cmake_minimum_required(VERSION 3.15.0)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sudoku VERSION 0.1.0)

include_directories(src)

# include test directory
add_subdirectory(tests)

# include .csv file
set(SAMPLE_CSV ${CMAKE_SOURCE_DIR}/sample.csv)
# Copy the sample.csv file to the build directory
configure_file(${SAMPLE_CSV} ${CMAKE_CURRENT_BINARY_DIR}/sample.csv COPYONLY)

# mark all .cpp files to be compiled
set(SOURCES "main.cpp")
aux_source_directory(src SOURCES)

# add the executable
add_executable(sudoku ${SOURCES})