cmake_minimum_required(VERSION 3.15.0)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sudoku VERSION 0.1.0)

find_package(Boost REQUIRED COMPONENTS container)
find_package(fmt CONFIG REQUIRED)

include_directories(src)

# include test directory
add_subdirectory(tests)

# include .csv file
set(SAMPLE_CSV ${CMAKE_SOURCE_DIR}/sample.csv)

# based on build type copy to build directory or debug directory
configure_file(${SAMPLE_CSV} ${CMAKE_CURRENT_BINARY_DIR}/Debug/sample.csv COPYONLY)
configure_file(${SAMPLE_CSV} ${CMAKE_CURRENT_BINARY_DIR}/sample.csv COPYONLY)

# mark all .cpp files to be compiled
set(SOURCES "main.cpp")
aux_source_directory(src SOURCES)

# add the executable
add_executable(sudoku ${SOURCES})

# link boost
target_link_libraries(sudoku PRIVATE Boost::container)
target_link_libraries(sudoku PRIVATE fmt::fmt)
