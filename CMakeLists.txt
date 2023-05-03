cmake_minimum_required(VERSION 3.15.0)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sudoku VERSION 0.1.0)

find_package(Boost REQUIRED COMPONENTS container log)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)


get_filename_component(DATA_FILE_ABS "data/sudoku-3m.csv" ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DDATA_FILE="${DATA_FILE_ABS}")

include_directories(src)


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
target_link_libraries(sudoku PRIVATE Boost::log)
target_link_libraries(sudoku PRIVATE fmt::fmt)
target_link_libraries(sudoku PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)


# add tests
enable_testing()
add_subdirectory(tests)

# discover tests in test directory
include(GoogleTest)
gtest_discover_tests(sudoku)
