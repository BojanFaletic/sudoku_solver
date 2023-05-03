cmake_minimum_required(VERSION 3.15.0)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sudoku VERSION 0.1.0)

# vcpkg required packages
find_package(Boost REQUIRED COMPONENTS container log)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# add path to .csv file
get_filename_component(DATA_FILE_ABS "data/sudoku-3m.csv" ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DDATA_FILE="${DATA_FILE_ABS}")

# add path to .csv file
get_filename_component(TEST_FILE_ABS "data/sample.csv" ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DTEST_FILE_ABS="${TEST_FILE_ABS}")

# mark all .cpp files to be compiled
include_directories(include/sud)
aux_source_directory(src/sud SOURCES)

# add the executable
add_executable(sudoku src/main.cpp ${SOURCES})

target_link_libraries(sudoku PRIVATE Boost::container Boost::log)
target_link_libraries(sudoku PRIVATE fmt::fmt)

# add tests
add_subdirectory(tests)
enable_testing()
