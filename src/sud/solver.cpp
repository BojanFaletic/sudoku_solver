#include "solver.hpp"

namespace sud
{
    Solver::Solver(Sudoku &sudoku) : sudoku(sudoku) {}
    RowSolver::RowSolver(Sudoku &sudoku) : Solver(sudoku) {}
    ColSolver::ColSolver(Sudoku &sudoku) : Solver(sudoku) {}
    BoxSolver::BoxSolver(Sudoku &sudoku) : Solver(sudoku) {}


    status_e RowSolver::solve()
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
