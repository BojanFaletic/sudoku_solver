#include "sud/sol/mustSolver.hpp"
#include <iostream>
#include <algorithm>

namespace sud::sol
{
    MustSolver::MustSolver(Sudoku &sudoku) : Common(sudoku)
    {
    }

    bool MustSolver::solve()
    {
        return false;
    }

    bool MustSolver::is_number_possible_outside_box_row(const Point &point, const Square value) const
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
			// is inside box
            if (row >= block.row && row < block.row + SUDOKU_BOX_SIZE)
            {
                continue;
            }

            if (possible[{row, point.col}].test(value))
            {
                return true;
            }
        }
        return false;
    }

    bool MustSolver::is_number_possible_outside_box_col(const Point &point, const Square value) const
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (uint8_t col = 0; col < SUDOKU_SIZE; col++)
        {
            if (col >= block.col && col < block.col + SUDOKU_BOX_SIZE)
            {
                continue;
            }
            if (possible[{point.row, col}].test(value))
            {
                return true;
            }
        }
        return false;
    }

    void MustSolver::remove_possible_candidate_inside_box_row(const Point &point, const Square value)
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (uint8_t row = block.row; row < block.row + SUDOKU_BOX_SIZE; row++)
        {
            if (row == point.row)
            {
                continue;
            }
            for (uint8_t col = block.col; col < block.col + SUDOKU_BOX_SIZE; col++)
            {
                possible[{row, col}][value] = false;
            }
        }
    }

    void MustSolver::remove_possible_candidate_inside_box_col(const Point &point, const Square value)
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (uint8_t col = block.col; col < block.col + SUDOKU_BOX_SIZE; col++)
        {
            if (col == point.col)
            {
                continue;
            }
            for (uint8_t row = block.row; row < block.row + SUDOKU_BOX_SIZE; row++)
            {
                possible[{row, col}][value] = false;
            }
        }
    }

    void MustSolver::filter_unique()
    {
        for (const Point &point : PointIterator())
        {
            //std::cout << fmt::format("filter_unique: point: {}\n", point);

            if (sudoku[point])
            {
                continue;
            }

            // break at (3,5)
            if (point.row == 3 && point.col == 5)
            {
                std::cout << "break\n";
                print_possible(point);
                print_possible({8, 5});
            }

            const std::vector<Square> possible_numbers = get_possible(point);
            for (const Square &number : possible_numbers)
            {
                bool can_be_outside_row = is_number_possible_outside_box_row(point, number);
                bool can_be_outside_col = is_number_possible_outside_box_col(point, number);
                //std::cout << fmt::format("filter_unique: point: {}, number: {}, can_be_outside_row: {}, can_be_outside_col: {}\n", point, number, can_be_outside_row, can_be_outside_col);

                if (!can_be_outside_row)
                {
                    remove_possible_candidate_inside_box_row(point, number);
                }

                if (!can_be_outside_col)
                {
                    remove_possible_candidate_inside_box_col(point, number);
                }
            }
        }
    }

} // namespace sud::sol
