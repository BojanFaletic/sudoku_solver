#include "solver.hpp"

namespace sud
{
    Solver::Solver(Sudoku &sudoku) : sudoku(sudoku) {}
    ColSolver::ColSolver(Sudoku &sudoku) : Solver(sudoku) {}
    BoxSolver::BoxSolver(Sudoku &sudoku) : Solver(sudoku) {}


    SimpleSolver::SimpleSolver(Sudoku &sudoku) : Solver(sudoku) {
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
