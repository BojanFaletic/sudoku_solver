#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "sud/sudoku.hpp"
#include "sud/sol/solver.hpp"
#include "sud/loader.hpp"
#include "sud/sol/simpleSolver.hpp"
#include "sud/sol/mustSolver.hpp"
#include <bitset>
#include <fmt/core.h>

using namespace std;
using namespace sud;
using namespace sud::sol;

std::array<std::array<Square, 9>, 9> const puzzle_1 = {
    {{1, 9, 8, 5, 0, 3, 7, 0, 6},
     {6, 0, 3, 0, 0, 8, 0, 9, 0},
     {0, 0, 7, 6, 0, 9, 8, 0, 0},
     {0, 1, 0, 0, 0, 0, 0, 6, 0},
     {8, 7, 6, 1, 0, 0, 0, 0, 0},
     {0, 3, 0, 0, 0, 6, 0, 7, 0},
     {0, 6, 0, 0, 0, 1, 9, 8, 7},
     {0, 8, 1, 9, 0, 7, 6, 5, 4},
     {7, 0, 9, 8, 6, 0, 3, 1, 2}}};

TEST(SimpleSolver, find_possible)
{
    Sudoku sudoku{TEST_FILE_ABS};
    SimpleSolver solver(sudoku);
    solver.find_possible();

	std::cout << "Original" << std::endl;
	std::cout << sudoku << std::endl;

    vector<Square> possible_expected_0_0{2, 9};
    EXPECT_EQ(solver.get_possible_from_sudoku({0, 0}), possible_expected_0_0);
}

TEST(SimpleSolver, check_xy)
{
    Sudoku sudoku{puzzle_1};

    EXPECT_EQ(sudoku[Point(7, 3)], Square(9));
}

TEST(SimpleSolver, simple_solver)
{
    Sudoku sudoku{puzzle_1};
    Sudoku sudoku2{puzzle_1};

    SimpleSolver solver(sudoku);
    EXPECT_EQ(solver.solve(), false);

    EXPECT_EQ(sudoku2, sudoku);
}

TEST(SimpleSolver, test_fn)
{
    Sudoku sudoku{puzzle_1};
    cout << "Original:" << endl;
    cout << sudoku << endl;
    MustSolver must(sudoku);

    must.print_possible_from_possible({0, 4});

    // fix
    bool test = must.possible[{0, 4}].test(1) == false;
    EXPECT_EQ(test, true);

    test = must.possible[{0, 4}].test(2) == true;
    EXPECT_EQ(test, true);

    test = must.possible[{0, 4}].test(4) == true;
    EXPECT_EQ(test, true);

    test = must.possible[{0, 4}].test(3) == false;
    EXPECT_EQ(test, true);

    //EXPECT_EQ(must.possible[{3, 0}].test(2) == true, true);
}

TEST(SimpleSolver, simple)
{
    Sudoku sudoku{puzzle_1};

    cout << "Original:" << endl;
    cout << sudoku << endl;
    MustSolver must(sudoku);

    const auto possible = must.get_possible_from_possible({0, 4});
    const auto expected = vector<Square>{2, 4};
    EXPECT_EQ(must.get_possible_from_possible({0, 4}), expected);

    const bool tst = must.is_number_possible_outside_box_row({3, 5}, 2);
    std::cout << "tst: " << tst << std::endl;

    ASSERT_FALSE(tst);
    ASSERT_TRUE(must.is_number_possible_outside_box_col({3, 5}, 2) == true);
}

TEST(SimpleSolver, must_solver_pre)
{
    Sudoku sudoku{puzzle_1};

    cout << "Original:" << endl;
    cout << sudoku << endl;
    MustSolver solver(sudoku);

    Point point{3, 5};
    int number = 2;
    bool can_be_outside_row = solver.is_number_possible_outside_box_row(point, number);
    ASSERT_FALSE(can_be_outside_row);


    solver.remove_possible_candidate_inside_box_row(point, number);

    // check that the number is not possible in the box
    auto possibe = solver.get_possible_from_possible({5,3});
    ASSERT_EQ(possibe, vector<Square>{4});


    point = {4, 7};
    number = 4;
    can_be_outside_row = solver.is_number_possible_outside_box_row(point, number);
    ASSERT_TRUE(can_be_outside_row);
}

TEST(SimpleSolver, must_solver)
{
    Sudoku sudoku{puzzle_1};

    cout << "Original:" << endl;
    cout << sudoku << endl;
    MustSolver solver(sudoku);
    solver.filter_unique();

    EXPECT_EQ(solver.get_possible_from_possible({5, 3}), vector<Square>{4});
    EXPECT_EQ(sudoku.is_valid(), true);
}

TEST(SimpleSolver, set_possible)
{
    Possible possible;
    possible[{0, 0}][1] = true;
    ASSERT_TRUE((possible[{0, 0}][1] == true));

    possible[{0, 0}][1] = false;
    ASSERT_TRUE((possible[{0, 0}][1] == false));
}

TEST(MustSolver, must_solve){
    Sudoku sudoku{puzzle_1};

    cout << "Original:" << endl;
    cout << sudoku << endl;
    MustSolver solver(sudoku);
    solver.single_solve();

    cout << "Solved:" << endl;
    cout << sudoku << endl;
    EXPECT_EQ(sudoku.is_valid(), true);
}

#if 0
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
#endif

#if 0
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
#endif

#if 0
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
#endif

#if 0
TEST(SimpleSolver, candidates_in_filled)
{
    Sudoku sudoku(puzzle_1);
    SimpleSolverTest solver(sudoku);

    cout << "Original:" << endl;
    cout << sudoku << endl;

    solver.find_possible();
    solver.update_possible();
    //solver.filter_unique();

    cout << "After find_possible and update_possible:" << endl;
    cout << sudoku << endl;

    vector<uint8_t> expected{};
    EXPECT_EQ(solver.get_possible({0, 0}), expected);
    expected = {2, 4};
    EXPECT_EQ(solver.get_possible({0, 4}), expected);

    expected = {4};
    EXPECT_EQ(solver.get_possible({5, 3}), expected);
}
#endif

#if 0
TEST(SimpleSolver, filter_unique)
{
    Sudoku sudoku(puzzle_1);
    SimpleSolverTest solver(sudoku);

    cout << "Original:" << endl;
    cout << sudoku << endl;

    solver.filter_unique();

    vector<uint8_t> expected{4};
    EXPECT_EQ(solver.get_possible({5, 4}), expected);
}
#endif

#if 0
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
#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
