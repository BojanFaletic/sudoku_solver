cmake_minimum_required(VERSION 3.20)

project(tests)

# include gtest with vcpkg
find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# include ../src
include_directories(../src)

# run main.cpp with gtest
add_executable(tests main.cpp ../src/sudoku.cpp)

# link gtest
target_link_libraries(tests PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_link_libraries(tests PRIVATE fmt::fmt)
