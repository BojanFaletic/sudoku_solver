#pragma once

#include "types.hpp"
#include "Sudoku.hpp"

namespace sud
{

    class Solver
    {
    protected:
        Sudoku &sudoku;
    public:
        Solver(Sudoku &sudoku);

        virtual status_e solve() = 0;
    };

    class RowSolver : public Solver
    {
    public:
        RowSolver(Sudoku &sudoku);

        status_e solve() override;
    };

    class ColSolver : public Solver
    {
    public:
        ColSolver(Sudoku &sudoku);

        status_e solve() override;
    };

    class BoxSolver : public Solver
    {
    public:
        BoxSolver(Sudoku &sudoku);

        status_e solve() override;
    };

}; // namespace sud