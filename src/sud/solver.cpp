#include "solver.hpp"
#include <algorithm>

namespace sud
{
    Solver::Solver(Sudoku &sudoku) : sudoku(sudoku) {}
    ColSolver::ColSolver(Sudoku &sudoku) : Solver(sudoku) {}
    BoxSolver::BoxSolver(Sudoku &sudoku) : Solver(sudoku) {}

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
                poss[row].set(sudoku.get(row, col));
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
                poss[col].set(sudoku.get(row, col));
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
                    poss[block_idx].set(sudoku.get(row + i, col + j));
                }
            }
            poss[block_idx].flip();
        }
        return poss;
    }

    bool SimpleSolver::unique_filter_row()
    {
        bool res = false;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            std::array<std::pair<uint8_t, uint8_t>, SUDOKU_POSSIBLE_NUMBERS> count;
            std::fill_n(count.begin(), SUDOKU_POSSIBLE_NUMBERS, std::make_pair(0, 0));

            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
                {
                    if (possible_board[row][col][i])
                    {
                        count[i].first++;
                        count[i].second = col;
                    }
                }
            }

            std::sort(count.begin(), count.end(), [](const std::pair<uint8_t, uint8_t> &a, const std::pair<uint8_t, uint8_t> &b)
                      { return a.first < b.first; });

            for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
            {
                if (count[i].first == 1)
                {
                    possible_board[row][count[i].second].reset();
                    possible_board[row][count[i].second].set(i);
                    res = true;
                }
                if (count[i].first > 1)
                {
                    return res;
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
            std::array<std::pair<uint8_t, uint8_t>, SUDOKU_POSSIBLE_NUMBERS> count;
            std::fill_n(count.begin(), SUDOKU_POSSIBLE_NUMBERS, std::make_pair(0, 0));

            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
                {
                    if (possible_board[row][col][i])
                    {
                        count[i].first++;
                        count[i].second = row;
                    }
                }
            }

            std::sort(count.begin(), count.end(), [](const std::pair<uint8_t, uint8_t> &a, const std::pair<uint8_t, uint8_t> &b)
                      { return a.first < b.first; });

            for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
            {
                if (count[i].first == 1)
                {
                    possible_board[count[i].second][col].reset();
                    possible_board[count[i].second][col].set(i);
                    res = true;
                }
                if (count[i].first > 1)
                {
                    return res;
                }
            }
        }
        return res;
    }

    bool SimpleSolver::unique_filter_box()
    {
        bool res = false;
        for (uint8_t block_idx = 0; block_idx < SUDOKU_SIZE; block_idx++)
        {
            const uint8_t row = block_idx / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
            const uint8_t col = block_idx % SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;

            std::array<std::pair<uint8_t, uint8_t>, SUDOKU_POSSIBLE_NUMBERS> count;
            std::fill_n(count.begin(), SUDOKU_POSSIBLE_NUMBERS, std::make_pair(0, 0));

            for (uint8_t i = 0; i < SUDOKU_BOX_SIZE; i++)
            {
                for (uint8_t j = 0; j < SUDOKU_BOX_SIZE; j++)
                {
                    for (uint8_t k = 1; k < SUDOKU_POSSIBLE_NUMBERS; k++)
                    {
                        if (possible_board[row + i][col + j][k])
                        {
                            count[k].first++;
                            count[k].second = (row + i) * SUDOKU_SIZE + col + j;
                        }
                    }
                }
            }

            std::sort(count.begin(), count.end(), [](const std::pair<uint8_t, uint8_t> &a, const std::pair<uint8_t, uint8_t> &b)
                      { return a.first < b.first; });

            for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
            {
                if (count[i].first == 1)
                {
                    possible_board[count[i].second / SUDOKU_SIZE][count[i].second % SUDOKU_SIZE].reset();
                    possible_board[count[i].second / SUDOKU_SIZE][count[i].second % SUDOKU_SIZE].set(i);
                    res = true;
                }
                if (count[i].first > 1)
                {
                    return res;
                }
            }
        }
        return res;
    }

    bool SimpleSolver::unique_filter()
    {
        return unique_filter_row() || unique_filter_col() || unique_filter_box();
    }

    void SimpleSolver::find_possible()
    {
        const possible_t row_wise = row_wise_possible();
        const possible_t col_wise = col_wise_possible();
        const possible_t box_wise = box_wise_possible();

        // for number to be valid, it must be present in all three sets
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                const uint8_t box_idx = (row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE + col / SUDOKU_BOX_SIZE;
                possible_board[row][col] = row_wise[row] & col_wise[col] & box_wise[box_idx];
            }
        }
    }

    status_e SimpleSolver::solve()
    {
        unique_filter();
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                if (sudoku.get(row, col) == 0)
                {
                    missing_t missing = sudoku.possible_items(row, col);
                    if (missing.size() == 1)
                    {
                        sudoku.set(row, col, *missing.begin());
                    }
                }
            }
        }

        return SUCCESS;
    }

    status_e ColSolver::solve()
    {
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                if (sudoku.get(row, col) == 0)
                {
                    missing_t missing = sudoku.possible_items(row, col);
                    if (missing.size() == 1)
                    {
                        sudoku.set(row, col, *missing.begin());
                    }
                }
            }
        }
        return SUCCESS;
    }

    status_e BoxSolver::solve()
    {
        for (square_t row = 0; row < SUDOKU_SIZE; row += SUDOKU_BOX_SIZE)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col += SUDOKU_BOX_SIZE)
            {
                for (square_t i = 0; i < SUDOKU_BOX_SIZE; i++)
                {
                    for (square_t j = 0; j < SUDOKU_BOX_SIZE; j++)
                    {
                        if (sudoku.get(row + i, col + j) == 0)
                        {
                            missing_t missing = sudoku.possible_items(row + i, col + j);
                            if (missing.size() == 1)
                            {
                                sudoku.set(row + i, col + j, *missing.begin());
                            }
                        }
                    }
                }
            }
        }
        return SUCCESS;
    }

} // namespace sud
