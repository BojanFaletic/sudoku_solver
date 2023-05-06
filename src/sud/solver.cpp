#include "solver.hpp"
#include <algorithm>
#include <cmath>
#include <cassert>
#include <fmt/format.h>
#include <iostream>

namespace sud
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
                for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
                {
                    if (possible_board[row][col][i])
                    {
                        count[i].freq++;
                        count[i].pos = {row, col};
                    }
                }
            }

            for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
            {
                if (count[i].freq == 1)
                {
                    insert(count[i].pos, i);
                    possible_board[count[i].pos.row][count[i].pos.col].reset();
                    res = true;
                }
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
                for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
                {
                    if (possible_board[row][col][i])
                    {
                        count[i].freq++;
                        count[i].pos = {row, col};
                    }
                }
            }

            for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
            {
                if (count[i].freq == 1)
                {
                    insert(count[i].pos, i);
                    possible_board[count[i].pos.row][count[i].pos.col].reset();
                    res = true;
                }
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
            if (it == 0){
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
        unique_filter_row();
        unique_filter_col();
        unique_filter_box();

        return false;
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
        for (const Point &point : PointIterator()){
            const uint8_t box_idx = (point.row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE + point.col / SUDOKU_BOX_SIZE;
            possible_board[point.row][point.col] = row_wise[point.row] & col_wise[point.col] & box_wise[box_idx];
        
            if (sudoku[point] != 0){
                possible_board[point.row][point.col].reset();
            }
        }
    }

    status_e SimpleSolver::solve()
    {
        unique_filter();
        return SUCCESS;
    }

} // namespace sud
