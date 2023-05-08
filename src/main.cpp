#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "sud/sudoku.hpp"
#include "sud/loader.hpp"
#include "sud/sol/solver.hpp"
#include "sud/sol/simpleSolver.hpp"
#include "fmt/format.h"


using namespace std;
using namespace sud;
using namespace sud::sol;
using namespace boost;

int main()
{
    // set up logging severity
    log::core::get()->set_filter(
        log::trivial::severity >= log::trivial::warning
    );

    
    Loader loader{DATA_FILE, 10};
    for (const auto &sudoku : loader.get_data())
    {
        cout << fmt::format("Solving puzzle {} with difficulty {}", sudoku.id, sudoku.difficulty.value_or(-1)) << endl;
        Sudoku s{sudoku.puzzle};
        //SimpleSolver solver{s};
        //solver.solve();
        cout << s << endl;
    }

    Sudoku sudoku{TEST_FILE_ABS};
    cout << sudoku << endl;

    cout << "Solving..." << endl;
    //SimpleSolver solver{sudoku};
    //solver.solve();
    cout << "Done" << endl;
    cout << "Final board:" << endl;
    cout << sudoku << endl;
    
    return 0;
    std::cout << "Saving..." << endl;
    sudoku.save_to_CSV("sample_solved.csv");

    //initBoard(sudoku.board);
    std::cout << "Hello World!\n";
    return 0;
}