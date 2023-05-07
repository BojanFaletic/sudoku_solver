#pragma once

#include "types.hpp"
#include "sudoku.hpp"
#include <bitset>
#include <array>
#include <vector>

namespace sud::sol
{
    class Solver
    {
    protected:
        Sudoku &sudoku;

    public:
        Solver(Sudoku &sudoku);

        virtual status_e solve() = 0;
    };

}; // namespace sud::sol