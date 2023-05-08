#include "sud/sol/common.hpp"
#include <iostream>

namespace sud::sol
{
    Common::Common(Sudoku &sudoku) : sudoku(sudoku), possible(sudoku)
    {
    }

    std::vector<square_t> Common::get_possible(const Point &point) const
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

    bool Common::is_number_possible_row(const Point &point, const square_t value) const
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

    bool Common::is_number_possible_col(const Point &point, const square_t value) const
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

    bool Common::is_number_possible_box(const Point &point, const square_t value) const
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

    bool Common::is_number_possible(const Point &point, const square_t value) const
    {
        bool possible = true;
        possible &= is_number_possible_row(point, value);
        possible &= is_number_possible_col(point, value);
        possible &= is_number_possible_box(point, value);
        return possible;
    }

    uint8_t Common::missing_number(const Point &point)
    {
        if (possible[point].count() == 1)
        {
            for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
            {
                if (possible[point][i])
                {
                    return i;
                }
            }
        }
        return 0;
    }

    Common::possible_array_t Common::row_wise_possible()
    {
        possible_array_t poss;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                poss[row].set(sudoku[{row, col}]);
            }
            poss[row].flip();
        }
        return poss;
    }

    Common::possible_array_t Common::col_wise_possible()
    {
        possible_array_t poss;
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                poss[col].set(sudoku[Point(row, col)]);
            }
            poss[col].flip();
        }
        return poss;
    }

    Common::possible_array_t Common::box_wise_possible()
    {
        possible_array_t poss;
        for (uint8_t block_idx = 0; block_idx < SUDOKU_SIZE; block_idx++)
        {
            const uint8_t row = block_idx / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
            const uint8_t col = block_idx % SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;

            for (uint8_t i = 0; i < SUDOKU_BOX_SIZE; i++)
            {
                for (uint8_t j = 0; j < SUDOKU_BOX_SIZE; j++)
                {
                    poss[block_idx].set(sudoku[Point(row + i, col + j)]);
                }
            }
            poss[block_idx].flip();
        }
        return poss;
    }

    void Common::find_possible()
    {
        const possible_array_t row_wise = row_wise_possible();
        const possible_array_t col_wise = col_wise_possible();
        const possible_array_t box_wise = box_wise_possible();

        // for number to be valid, it must be present in all three sets
        for (const Point &point : PointIterator())
        {
            const uint8_t box_idx = (point.row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE + point.col / SUDOKU_BOX_SIZE;
            possible[point] = row_wise[point.row] & col_wise[point.col] & box_wise[box_idx];

            if (sudoku[point] != 0)
            {
                possible[point].reset();
            }
        }
    }

    bool Common::basic_solve()
    {
        bool res = false;
        for (const Point &point : PointIterator())
        {
            if (sudoku[point] == 0)
            {
                const uint8_t val = missing_number(point);
                if (val != 0)
                {
                    sudoku[point] = val;
                    res = true;
                }
            }
        }
        return res;
    }
}