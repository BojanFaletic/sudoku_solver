#include <gtest/gtest.h>

#include "Sudoku.hpp"
#include "Loader.hpp"
#include "Solver.hpp"
#include <vector>
#include <array>
#include <fmt/format.h>

using namespace std;
using namespace sud;

TEST(Loader, constructor)
{
    cout << TEST_FILE_ABS << endl;
    Sudoku sudoku{TEST_FILE_ABS};
}

TEST(Loader, load)
{
    Loader loader{DATA_FILE, 10};
    for (const auto &sudoku : loader.get_data())
    {
        cout << fmt::format("Solving puzzle {} with difficulty {}", sudoku.id, sudoku.difficulty.value_or(-1)) << endl;
        Sudoku s{sudoku.puzzle};
        SimpleSolver solver{s};
        solver.solve();
        cout << s << endl;
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}