#include "sud//sol/simpleSolver.hpp"
#include <iostream>
#include <cmath>

namespace sud::sol
{

    SimpleSolver::SimpleSolver(Sudoku &sudoku) : Common(sudoku)
    {
    }

    bool SimpleSolver::unique_filter_row()
    {
        bool res = false;
        for (std::uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> count{};

            for (std::uint8_t col = 0; col < SUDOKU_SIZE; col++)
            {
                update_freq({row, col}, count);
            }

            const uint8_t it = freq_to_value(count);
            if (it != 0)
            {
                sudoku[count[it].pos] = it;
                possible[count[it].pos].reset();
                res = true;
            }
        }
        return res;
    }

    bool SimpleSolver::unique_filter_col()
    {
        bool res = false;
        for (std::uint8_t col = 0; col < SUDOKU_SIZE; col++)
        {
            std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> count{};

            for (std::uint8_t row = 0; row < SUDOKU_SIZE; row++)
            {
                update_freq({row, col}, count);
            }

            const uint8_t it = freq_to_value(count);
            if (it != 0)
            {
                sudoku[count[it].pos] = it;
                possible[count[it].pos].reset();
                res = true;
            }
        }
        return res;
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
                sudoku[point] = it;
                possible[point].reset();
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
            if (possible[point][i])
            {
                count[i].freq++;
                count[i].pos = point;
            }
        }
    }

    bool SimpleSolver::unique_filter()
    {
        bool res = false;
        res |= unique_filter_row();
        res |= unique_filter_col();
        res |= unique_filter_box();
        return res;
    }

    void SimpleSolver::update_possible()
    {
        for (const Point &pt : PointIterator())
        {
            if (sudoku[pt] != 0)
            {
                const uint8_t missing = missing_number(pt);
                if (missing != 0)
                {
                    possible[pt].reset();
                    possible[pt] = missing;
                }
            }
        }
    }

    bool SimpleSolver::solve()
    {
        bool inserted_items, res = false;
        do
        {
            inserted_items = false;
            inserted_items |= unique_filter();
            inserted_items |= basic_solve();
            res |= inserted_items;
        } while (inserted_items);
        return res;
    }

} // namespace sud::sol