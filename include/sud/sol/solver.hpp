#pragma once

#include "types.hpp"
#include "sudoku.hpp"
#include "simple.hpp"
#include "must.hpp"
#include <array>

namespace sud::sol
{
    class Solver
    {
    private:
        Simple simple;
        Must must;
        Sudoku &sudoku;

    public:
        Solver(Sudoku &sudoku);

        status_e solve();
    };

}; // namespace sud::sol