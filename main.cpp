#include <iostream>

#include "sudoku.hpp"

using namespace std;
using namespace sud;

int main()
{
    Sudoku sudoku{"sample.csv"};
    cout << sudoku << endl;

    cout << "Solving..." << endl;
    sudoku.solve();
    cout << sudoku << endl;
    
    //initBoard(sudoku.board);
    std::cout << "Hello World!\n";
    return 0;
}