#pragma once

#include "types.hpp"
#include "Sudoku.hpp"
#include <bitset>
#include <array>

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


    class SimpleSolver : public Solver
    {
        friend class SimpleSolverTest; 
    private:
        using possible_t = std::array<std::bitset<SUDOKU_POSSIBLE_NUMBERS>, SUDOKU_SIZE>;
        std::array<possible_t, SUDOKU_SIZE> possible_board;

        possible_t row_wise_possible();
        possible_t col_wise_possible();
        possible_t box_wise_possible();

        void find_possible();

        void unique_solver();

    public:
        SimpleSolver(Sudoku &sudoku);

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