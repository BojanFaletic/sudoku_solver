#include "solver.hpp"
#include "types.hpp"
#include <algorithm>
#include <cmath>
#include <cassert>
#include <fmt/format.h>
#include <iostream>

namespace sud::sol
{
    Solver::Solver(Sudoku &sudoku) : sudoku(sudoku) {}

    SimpleSolver::SimpleSolver(Sudoku &sudoku) : Solver(sudoku)
    {
        find_possible();
    }

    SimpleSolver::possible_t SimpleSolver::row_wise_possible()
    {
        possible_t poss;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                poss[row].set(sudoku[Point(row, col)]);
            }
            poss[row].flip();
        }
        return poss;
    }

    SimpleSolver::possible_t SimpleSolver::col_wise_possible()
    {
        possible_t poss;
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

    SimpleSolver::possible_t SimpleSolver::box_wise_possible()
    {
        possible_t poss;
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

    uint8_t SimpleSolver::missing_number(const Point &point)
    {
        if (possible_board[point.row][point.col].count() == 1)
        {
            for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
            {
                if (possible_board[point.row][point.col][i])
                {
                    return i;
                }
            }
        }
        return 0;
    }

    bool SimpleSolver::basic_solve()
    {
        bool res = false;

        for (const Point &point : PointIterator())
        {
            if (sudoku[point] == 0)
            {
                const uint8_t val = missing_number(point);
                if (val != 0)
                {
                    insert(point, val);
                    res = true;
                }
            }
        }
        return res;
    }

    void SimpleSolver::insert(const Point &point, const square_t value)
    {
        std::cout << fmt::format("Inserting {} at ({}, {})\n", value, point.row, point.col);
        assertm(sudoku[point] == 0, "Trying to insert a value in a non-empty square");
        sudoku[point] = value;
        // todo update possible_board
    }

    bool SimpleSolver::unique_filter_row()
    {
        bool res = false;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> count{};

            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                update_freq({row, col}, count);
            }

            const uint8_t it = freq_to_value(count);
            if (it != 0)
            {
                insert(count[it].pos, it);
                possible_board[count[it].pos.row][count[it].pos.col].reset();
                res = true;
            }
        }
        return res;
    }

    bool SimpleSolver::unique_filter_col()
    {
        bool res = false;
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> count{};

            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                update_freq({row, col}, count);
            }

            const uint8_t it = freq_to_value(count);
            if (it != 0)
            {
                insert(count[it].pos, it);
                possible_board[count[it].pos.row][count[it].pos.col].reset();
                res = true;
            }
        }
        return res;
    }

    uint8_t SimpleSolver::freq_to_value(const std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> &count)
    {
        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (count[i].freq == 1)
            {
                return i;
            }
        }
        return 0;
    }

    void SimpleSolver::update_freq(const Point &point, std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> &count) const
    {
        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (possible_board[point.row][point.col][i])
            {
                count[i].freq++;
                count[i].pos = point;
            }
        }
    }

    bool SimpleSolver::unique_filter_box()
    {
        bool res = false;
        for (uint8_t block_idx = 0; block_idx < SUDOKU_SIZE; block_idx++)
        {
            const Point block = Point(block_idx / SUDOKU_BOX_SIZE, block_idx % SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
            std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> count{};

            for (uint8_t i = 0; i < SUDOKU_BOX_SIZE; i++)
            {
                for (uint8_t j = 0; j < SUDOKU_BOX_SIZE; j++)
                {
                    const Point point = block + Point(i, j);
                    update_freq(point, count);
                }
            }

            const uint8_t it = freq_to_value(count);
            if (it == 0)
            {
                continue;
            }
            Point point = count[it].pos;
            if (sudoku[point] == 0)
            {
                insert(point, it);
                possible_board[point.row][point.col].reset();
                res = true;
            }
        }
        return res;
    }

    bool SimpleSolver::unique_filter()
    {
        bool res = false;
        res |= unique_filter_row();
        res |= unique_filter_col();
        res |= unique_filter_box();
        return res;
    }

    bool SimpleSolver::is_number_possible_row(const Point &point, const square_t value) const{
        for (uint8_t col = 0; col < SUDOKU_SIZE; col++)
        {
            if (sudoku[{point.row, col}] == value)
            {
                return false;
            }
        }
        return true;
    }

    bool SimpleSolver::is_number_possible_col(const Point &point, const square_t value) const{
        for (uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            if (sudoku[{row, point.col}] == value)
            {
                return false;
            }
        }
        return true;
    }

    bool SimpleSolver::is_number_possible_box(const Point &point, const square_t value) const{
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

    bool SimpleSolver::is_number_possible(const Point &point, const square_t value) const{
        bool possible = true;
        possible &= is_number_possible_row(point, value);
        possible &= is_number_possible_col(point, value);
        possible &= is_number_possible_box(point, value);
        return possible;
    }

    bool SimpleSolver::is_number_possible_outside_box_row(const Point &point, const square_t value) const
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

    bool SimpleSolver::is_number_possible_outside_box_col(const Point &point, const square_t value) const
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

    void SimpleSolver::remove_possible_candidate_inside_box_row(const Point &point, const square_t value)
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
                if (possible_board[row][col][value])
                {
                    std::cout << fmt::format("remove_possible_candidate_inside_box_row: point: {}, value: {}\n", Point(row, col), value);
                }

                possible_board[row][col][value] = false;

            }
        }
    }

    void SimpleSolver::remove_possible_candidate_inside_box_col(const Point &point, const square_t value)
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
                possible_board[row][col][value] = false;
            }
        }
    }

    void SimpleSolver::filter_unique()
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
                    if (possible_board[row][col][number])
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

    void SimpleSolver::update_possible()
    {
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                if (sudoku.get(row, col) == 0)
                {
                    // if there is only one possible number, set it
                    if (possible_board[row][col].count() == 1)
                    {
                        uint8_t num = std::log2(possible_board[row][col].to_ulong());
                        sudoku.set(row, col, num);
                    }
                }
            }
        }
    }

    void SimpleSolver::find_possible()
    {
        const possible_t row_wise = row_wise_possible();
        const possible_t col_wise = col_wise_possible();
        const possible_t box_wise = box_wise_possible();

        // for number to be valid, it must be present in all three sets
        for (const Point &point : PointIterator())
        {
            const uint8_t box_idx = (point.row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE + point.col / SUDOKU_BOX_SIZE;
            possible_board[point.row][point.col] = row_wise[point.row] & col_wise[point.col] & box_wise[box_idx];

            if (sudoku[point] != 0)
            {
                possible_board[point.row][point.col].reset();
            }
        }
    }

    std::vector<square_t> SimpleSolver::get_possible(const Point &point) const
    {
        std::vector<square_t> possible;
        for (uint8_t number = 1; number < SUDOKU_POSSIBLE_NUMBERS; number++)
        {
            if (possible_board[point.row][point.col][number])
            {
                possible.push_back(number);
            }
        }
        return possible;
    }

    status_e SimpleSolver::solve()
    {
        unique_filter();
        return SUCCESS;
    }

} // namespace sud
