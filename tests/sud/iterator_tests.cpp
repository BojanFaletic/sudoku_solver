#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "Sudoku.hpp"
#include "Solver.hpp"
#include "Loader.hpp"
#include <bitset>
#include <fmt/core.h>
#include "types.hpp"

using namespace std;
using namespace sud;


TEST(PointIterator, basic_loop)
{
    bool visited[9][9] = {false};
    for (const Point &p : PointIterator())
    {
        visited[p.row][p.col] = true;

    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j< 9; j++)
        {
            ASSERT_TRUE(visited[i][j]);
        }
    }
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}