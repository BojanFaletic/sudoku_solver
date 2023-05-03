# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED COMPONENTS container log)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# add all src/*.cpp files to the executable
set(SOURCES "main.cpp")
aux_source_directory(../src SOURCES)

# create a test executable
add_executable(sudoku_tests ${SOURCES})

# link to the sudoku executable and GTest libraries
target_link_libraries(sudoku_tests PRIVATE Boost::container)
target_link_libraries(sudoku_tests PRIVATE Boost::log)
target_link_libraries(sudoku_tests PRIVATE fmt::fmt)
target_link_libraries(sudoku_tests PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

# add the tests
include(GoogleTest)
gtest_discover_tests(sudoku_tests)


# copy the sample csv file to the build directory
set(SAMPLE_CSV ../sample.csv)
configure_file(${SAMPLE_CSV} ${CMAKE_CURRENT_BINARY_DIR}/Debug/sample.csv COPYONLY)

