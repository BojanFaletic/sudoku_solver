#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "Sudoku.hpp"
#include "Solver.hpp"
#include "Loader.hpp"
#include <bitset>
#include <fmt/core.h>

namespace sud
{
    class SimpleSolverTest : public SimpleSolver
    {
    public:
        using SimpleSolver::find_possible;
        using SimpleSolver::possible_board;
        using SimpleSolver::SimpleSolver;

        using SimpleSolver::unique_filter_box;
        using SimpleSolver::unique_filter_col;
        using SimpleSolver::unique_filter_row;
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

vector<uint8_t> bitset2vect(bitset<10> bitset)
{
    vector<uint8_t> result;
    for (int i = 0; i < 10; i++)
    {
        if (bitset.test(i))
        {
            result.push_back(i);
        }
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

TEST(SimpleSolverTest, box_filter)
{
    Loader loader{DATA_FILE, 10};
    Sudoku sudoku{loader.get_puzzle(1).value()};
    SimpleSolverTest solver(sudoku);

    cout << "Original:" << endl;
    cout << sudoku << endl;

    // do a few iterations of normal solver
    for (int i = 0; i < 10; i++)
    {
        solver.find_possible();
        bool res = solver.basic_solve();
        if (!res)
        {
            break;
        }

        cout << fmt::format("After find_possible and update_possible: {}", i) << endl;
        cout << sudoku << endl;
    } 

    cout << "Before unique_filter_box" << endl;
    solver.find_possible();
    solver.unique_filter_box();
    cout << sudoku << endl;
    vector<uint8_t> expected{6};
    EXPECT_EQ(bitset2vect(solver.possible_board[7][3]), expected);
}

TEST(SimpleSolverTest, solve_full_simple)
{
    Sudoku sudoku{TEST_FILE_ABS};
    SimpleSolverTest solver(sudoku);

    cout << "Original:" << endl;
    cout << sudoku << endl;

    const int max_iter = 10;
    for (int i = 0; i < max_iter; i++)
    {
        solver.find_possible();
        solver.update_possible();

        cout << fmt::format("After find_possible and update_possible: {}", i) << endl;
        cout << sudoku << endl;

        EXPECT_EQ(sudoku.check(), 0);
        if (sudoku.is_solved())
        {
            break;
        }
    }

    EXPECT_EQ(sudoku.is_solved(), true);
}

TEST(SimpleSolverTest, solve_full_normal)
{
    Loader loader{DATA_FILE, 10};
    Sudoku sudoku(loader.get_puzzle(1).value());
    SimpleSolverTest solver(sudoku);

    cout << "Original:" << endl;
    cout << sudoku << endl;

    const int max_iter = 10;
    for (int i = 0; i < max_iter; i++)
    {
        solver.find_possible();
        solver.unique_filter();
        solver.update_possible();

        cout << fmt::format("After find_possible and update_possible: {}", i) << endl;
        cout << sudoku << endl;

        EXPECT_EQ(sudoku.check(), 0);

        if (sudoku.is_solved())
        {
            break;
        }
    }

    EXPECT_EQ(sudoku.is_solved(), true);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
