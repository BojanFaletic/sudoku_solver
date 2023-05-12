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
            if (row >= block.row && row < block.row + SUDOKU_BOX_SIZE)
            {
                continue;
            }
            bool is_possible = possible[{row, point.col}].test(value);
            if (is_possible)
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
            auto ps = possible[{point.row, col}];
            bool is_possible = ps.test(value);
            if (is_possible)
            {
                return true;
            }
        }
        return false;
    }

    void MustSolver::remove_possible_candidate_inside_box_row(const Point &point, const Square value)
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        std::vector<uint8_t> col_iterator{0, 1, 2};
        col_iterator.erase(col_iterator.begin() + point.col % 3);

        for (uint8_t row = block.row; row < block.row + SUDOKU_BOX_SIZE; row++)
        {
            std::for_each(col_iterator.begin(), col_iterator.end(), [&](uint8_t &col) {
                possible[{row, col}][value] = false;
            });
        }
    }

    void MustSolver::remove_possible_candidate_inside_box_col(const Point &point, const Square value)
    {
        const Point block = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        std::vector<uint8_t> row_iterator{0, 1, 2};
        row_iterator.erase(row_iterator.begin() + point.row % 3);

        for (uint8_t col = block.col; col < block.col + SUDOKU_BOX_SIZE; col++)
        {
            std::for_each(row_iterator.begin(), row_iterator.end(), [&](uint8_t &row) {
                possible[{row, col}][value] = false;
            });
        }
    }

    void MustSolver::filter_unique()
    {
        for (const Point &point : PointIterator())
        {
            std::cout << fmt::format("filter_unique: point: {}\n", point);

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
