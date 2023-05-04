#pragma once

#include "types.hpp"
#include "Sudoku.hpp"

namespace sud
{

    class Solver
    {
    public:
        Solver(Sudoku &sudoku);

        virtual status_e solve() = 0;
    };

}; // namespace sud