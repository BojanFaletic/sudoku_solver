# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# vcpkg required packages
find_package(Boost REQUIRED COMPONENTS container log)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# mark all .cpp files to be compiled
include_directories(../include/sud)
aux_source_directory(../src/sud SOURCES2)

# add tests
add_executable(loader_tests sud/loader_tests.cpp ${SOURCES2})
add_executable(solver_tests sud/solver_tests.cpp ${SOURCES2})
add_executable(iterator_tests sud/iterator_tests.cpp ${SOURCES2})


# link libraries
set(LIBS Boost::container Boost::log fmt::fmt GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_link_libraries(loader_tests PRIVATE ${LIBS})
target_link_libraries(solver_tests PRIVATE ${LIBS})
target_link_libraries(iterator_tests PRIVATE ${LIBS})

# add tests to CTest
include(GoogleTest)
gtest_discover_tests(loader_tests)
gtest_discover_tests(solver_tests)
gtest_discover_tests(iterator_tests)
