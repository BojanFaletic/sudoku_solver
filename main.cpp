#include <iostream>

#include "sudoku.hpp"

using namespace std;
using namespace sud;

int main()
{
    Sudoku sudoku;
    sudoku.load_from_CSV("sample.csv");
    cout << sudoku << endl;
    
    //initBoard(sudoku.board);
    std::cout << "Hello World!\n";
    return 0;
}