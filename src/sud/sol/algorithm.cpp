#include "sud/sol/algorithm.hpp"

namespace sud::sol
{
    Algorithm::Algorithm(SolverSudoku &sudoku) : sudoku(sudoku) {}

    std::vector<square_t> Algorithm::get_possible(const Point &point) const
    {
        std::vector<square_t> possible;
        for (square_t i = 1; i <= 9; i++)
        {
            if (sudoku.is_possible(point, i))
            {
                possible.push_back(i);
            }
        }
        return possible;
    }

    bool Algorithm::is_number_possible_row(const Point &point, const square_t value) const
    {
        for (uint8_t col = 0; col < SUDOKU_SIZE; col++)
        {
            if (sudoku[{point.row, col}] == value)
            {
                return false;
            }
        }
        return true;
    }

    bool Algorithm::is_number_possible_col(const Point &point, const square_t value) const
    {
        for (uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            if (sudoku[{row, point.col}] == value)
            {
                return false;
            }
        }
        return true;
    }

    bool Algorithm::is_number_possible_box(const Point &point, const square_t value) const
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (uint8_t i = 0; i < SUDOKU_BOX_SIZE; i++)
        {
            for (uint8_t j = 0; j < SUDOKU_BOX_SIZE; j++)
            {
                const Point p = block + Point(i, j);
                if (sudoku[p] == value)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool Algorithm::is_number_possible(const Point &point, const square_t value) const
    {
        bool possible = true;
        possible &= is_number_possible_row(point, value);
        possible &= is_number_possible_col(point, value);
        possible &= is_number_possible_box(point, value);
        return possible;
    }
}