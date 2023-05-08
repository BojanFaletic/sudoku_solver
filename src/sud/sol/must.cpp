#include "sud/sol/must.hpp"
#include <iostream>

namespace sud::sol
{
    Must::Must(Sudoku &sudoku) : Algorithm(sudoku)
    {
    }

    bool Must::solve()
    {
        return false;
    }

    bool Must::is_number_possible_outside_box_row(const Point &point, const square_t value) const
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            if (row >= block.row && row < block.row + SUDOKU_BOX_SIZE)
            {
                continue;
            }
            if (is_number_possible({row, point.col}, value))
            {
                return true;
            }
        }
        return false;
    }

    bool Must::is_number_possible_outside_box_col(const Point &point, const square_t value) const
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (uint8_t col = 0; col < SUDOKU_SIZE; col++)
        {
            if (col >= block.col && col < block.col + SUDOKU_BOX_SIZE)
            {
                continue;
            }
            if (is_number_possible({point.row, col}, value))
            {
                return true;
            }
        }
        return false;
    }

    void Must::remove_possible_candidate_inside_box_row(const Point &point, const square_t value)
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;

        std::cout << fmt::format("remove_possible_candidate_inside_box_row: point: {}, value: {}\n", block, value);
        for (uint8_t row = block.row; row < block.row + SUDOKU_BOX_SIZE; row++)
        {
            for (uint8_t col = block.col; col < block.col + SUDOKU_BOX_SIZE; col++)
            {
                if (row == point.row)
                {
                    continue;
                }
                if (possible[{row, col}][value])
                {
                    std::cout << fmt::format("remove_possible_candidate_inside_box_row: point: {}, value: {}\n", Point(row, col), value);
                }

                possible[{row, col}][value] = false;
            }
        }
    }

    void Must::remove_possible_candidate_inside_box_col(const Point &point, const square_t value)
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;

        std::cout << fmt::format("remove_possible_candidate_inside_box_col: point: {}, value: {}\n", block, value);
        for (uint8_t row = block.row; row < block.row + SUDOKU_BOX_SIZE; row++)
        {
            for (uint8_t col = block.col; col < block.col + SUDOKU_BOX_SIZE; col++)
            {
                if (col == point.col)
                {
                    continue;
                }
                possible[{row, col}][value] = false;
            }
        }
    }

    void Must::filter_unique()
    {
        // check if the number must be in a row
        for (uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (uint8_t col = 0; col < SUDOKU_SIZE; col++)
            {

                if (sudoku[{row, col}] != 0)
                {
                    continue;
                }

                // break at (3,5)
                if (row == 3 && col == 5)
                {
                    std::cout << "break\n";
                }

                for (uint8_t number = 1; number < SUDOKU_POSSIBLE_NUMBERS; number++)
                {
                    if (possible[{row, col}][number])
                    {
                        bool is_outside_row = is_number_possible_outside_box_row({row, col}, number);
                        bool is_outside_col = is_number_possible_outside_box_col({row, col}, number);

                        if (!is_outside_row)
                        {
                            // number must be in the row
                            // remove it from the other boxes in the row
                            remove_possible_candidate_inside_box_row({row, col}, number);
                        }

                        if (!is_outside_col)
                        {
                            // number must be in the col
                            // remove it from the other boxes in the col
                            remove_possible_candidate_inside_box_col({row, col}, number);
                        }
                    }
                }
            }
        }
    }

} // namespace sud::sol
