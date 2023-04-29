#include <gtest/gtest.h>

#include "Sudoku.hpp"
#include <vector>
#include <array>

using namespace std;
using namespace sud;

TEST(Sudoku, constructor)
{
    Sudoku sudoku = Sudoku();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
