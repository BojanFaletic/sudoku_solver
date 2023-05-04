#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "Sudoku.hpp"
#include "Solver.hpp"
#include <bitset>

namespace sud
{
    class SimpleSolverTest : public SimpleSolver
    {
    public:
        using SimpleSolver::find_possible;
        using SimpleSolver::possible_board;
        using SimpleSolver::SimpleSolver;
    };

}

using namespace std;
using namespace sud;

bitset<10> vect2bitset(vector<uint8_t> vect)
{
    bitset<10> result{0b0000000000};
    for (auto &i : vect)
    {
        result.set(i);
    }
    return result;
}

TEST(SimpleSolverTest, find_possible)
{
    Sudoku sudoku{TEST_FILE_ABS};
    SimpleSolverTest solver(sudoku);
    solver.find_possible();

    bitset<10> possible_expected_0_0{vect2bitset({2, 9})};
    EXPECT_EQ(solver.possible_board[0][0], possible_expected_0_0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
