#include <gtest/gtest.h>

#include "sud/sudoku.hpp"
#include "sud/loader.hpp"
#include "sud/sol/simpleSolver.hpp"
#include <vector>
#include <array>
#include <fmt/format.h>

using namespace std;
using namespace sud;
using namespace sud::sol;

TEST(Loader, constructor)
{
    cout << TEST_FILE_ABS << endl;
    Sudoku sudoku{TEST_FILE_ABS};
    cout << sudoku << endl;
    EXPECT_TRUE(sudoku.check() == SUCCESS);
}

TEST(Loader, load)
{
    Loader loader{DATA_FILE, 10};
    for (const auto &sudoku : loader.get_data())
    {
        const string msg = fmt::format("Solving puzzle {} with difficulty {}", sudoku.id, sudoku.difficulty.value_or(-1));
        cout << msg << endl;
        Sudoku s{sudoku.puzzle};
        SimpleSolver solver{s};
        solver.solve();
        cout << s << endl;
        EXPECT_TRUE(s.check() == SUCCESS);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
