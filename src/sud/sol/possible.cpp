#include "sud/sol/possible.hpp"

namespace sud::sol
{
    Possible::Possible(Sudoku &sudoku) : sudoku(sudoku)
    {
    }

    possible_t &Possible::operator[](const Point &p)
    {
        return possible_board[p.row][p.col];
    }

    const possible_t &Possible::operator[](const Point &p) const
    {
        return possible_board[p.row][p.col];
    }
}