#include <gtest/gtest.h>

#include "Sudoku.hpp"
#include <vector>
#include <array>


using namespace std;
using namespace sud;

TEST(Primitives, possible_items){
    sudoku_t sud;
    init_board(sud);

    missing_t result = possible_items(sud, 0, 0);
    missing_t expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(result, expected);

    sud[0][0] = 1;
    result = possible_items(sud, 0, 0);
    expected = {2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(result, expected);
}


TEST(Sudoku, constructor)
{
    Sudoku sudoku = Sudoku();
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
