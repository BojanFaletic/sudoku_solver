#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "sudoku.hpp"
#include "loader.hpp"
#include "fmt/format.h"


using namespace std;
using namespace sud;
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
        s.solve();
        cout << s << endl;
    }

    Sudoku sudoku{"sample.csv"};
    cout << sudoku << endl;

    cout << "Solving..." << endl;
    sudoku.solve();
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