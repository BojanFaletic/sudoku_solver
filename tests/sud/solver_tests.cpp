#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "sudoku.hpp"
#include "solver.hpp"
#include "loader.hpp"
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

    // test box filter
    for (int i = 0; i < 10; i++)
    {
        solver.find_possible();
        bool res = solver.unique_filter_box();
        if (!res)
        {
            break;
        }
        cout << fmt::format("After find_possible and unique_filter_box: {}", i) << endl;
        cout << sudoku << endl;
    }

    // test row filter
    for (int i = 0; i < 10; i++)
    {
        solver.find_possible();
        bool res = solver.unique_filter();
        if (!res)
        {
            break;
        }
        cout << fmt::format("After find_possible and unique_filter: {}", i) << endl;
        cout << sudoku << endl;
    }

    cout << "Final:" << endl;
    cout << sudoku << endl;

    solver.find_possible();
    vector<uint8_t> expected{1, 2};
    EXPECT_EQ(bitset2vect(solver.possible_board[7][2]), expected);
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

TEST(SimpleSolver, filter_unique)
{
    std::array<std::array<uint8_t, 9>, 9> puzzle = {
        {{1, 9, 8, 5, 0, 3, 7, 0, 6},
         {6, 0, 3, 0, 0, 8, 0, 9, 0},
         {0, 0, 7, 6, 0, 9, 8, 0, 0},
         {0, 1, 0, 0, 0, 0, 0, 6, 0},
         {8, 7, 6, 1, 0, 0, 0, 0, 0},
         {0, 3, 0, 0, 0, 6, 0, 7, 0},
         {0, 6, 0, 0, 0, 1, 9, 8, 7},
         {0, 8, 1, 9, 0, 7, 6, 5, 4},
         {7, 0, 9, 8, 6, 0, 3, 1, 2}}};

    Sudoku sudoku(puzzle);
    SimpleSolverTest solver(sudoku);

    cout << "Original:" << endl;
    cout << sudoku << endl;

    solver.filter_unique();

    vector<uint8_t> expected{4};
    EXPECT_EQ(bitset2vect(solver.possible_board[5][3]), expected);
}

TEST(SimpleSolver, filter_unique2)
{
    std::array<std::array<uint8_t, 9>, 9> puzzle = {
        {{1, 9, 8, 5, 0, 3, 7, 0, 6},
         {6, 0, 3, 0, 0, 8, 0, 9, 0},
         {0, 0, 7, 6, 0, 9, 8, 0, 0},
         {0, 1, 0, 0, 0, 0, 0, 6, 0},
         {8, 7, 6, 1, 0, 0, 0, 0, 0},
         {0, 3, 0, 0, 0, 6, 0, 7, 0},
         {0, 6, 0, 0, 0, 1, 9, 8, 7},
         {0, 8, 1, 9, 0, 7, 6, 5, 4},
         {7, 0, 9, 8, 6, 0, 3, 1, 2}}};

    Sudoku sudoku(puzzle);
    SimpleSolverTest solver(sudoku);

    cout << "Original:" << endl;
    cout << sudoku << endl;

    solver.filter_unique();

    for (int i = 0; i < 10; i++)
    {
        if (i != 0){
            solver.find_possible();
        }
        solver.unique_filter();
        solver.update_possible();

        cout << fmt::format("After find_possible and update_possible: {}", i) << endl;
        cout << sudoku << endl;

        ASSERT_EQ(sudoku.is_valid(), true);
        EXPECT_EQ(sudoku.check(), 0);

        if (sudoku.is_solved())
        {
            break;
        }
    }

    cout << "Final:" << endl;
    cout << sudoku << endl;

    vector<uint8_t> expected{4};
    EXPECT_EQ(true, sudoku.is_solved());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
