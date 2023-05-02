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