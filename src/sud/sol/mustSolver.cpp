#include "sud/sol/mustSolver.hpp"
#include <iostream>

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
            if (is_number_possible({row, point.col}, value))
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
            if (is_number_possible({point.row, col}, value))
            {
                return true;
            }
        }
        return false;
    }

    void MustSolver::remove_possible_candidate_inside_box_row(const Point &point, const Square value)
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
                if (possible[{row, col}][value.to_value()])
                {
                    std::cout << fmt::format("remove_possible_candidate_inside_box_row: point: {}, value: {}\n", Point(row, col), value);
                }

                possible[{row, col}][value.to_value()] = false;
            }
        }
    }

    void MustSolver::remove_possible_candidate_inside_box_col(const Point &point, const Square value)
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

    void MustSolver::filter_unique()
    {
        for (const Point &point : PointIterator())
        {
            std::cout << fmt::format("filter_unique: point: {}\n", point);

            if (!sudoku[point])
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
                bool is_unique_in_row = is_number_possible_outside_box_row(point, number);
                bool is_unique_in_col = is_number_possible_outside_box_col(point, number);

                if (is_unique_in_row || is_unique_in_col)
                {
                    sudoku[point] = number;
                    std::cout << fmt::format("filter_unique: point: {}, value: {}\n", point, number);
                    print_possible(point);
                    print_possible({8, 5});
                    //return;
                }
            }
        }
    }

} // namespace sud::sol
