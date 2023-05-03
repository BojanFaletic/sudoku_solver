#include "Sudoku.hpp"
#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>


using namespace std;
using namespace sud;

TEST(Solver, constructor)
{
    cout << TEST_FILE_ABS << endl;
    Sudoku sudoku{TEST_FILE_ABS};
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
