#pragma once

#include "sud/types.hpp"
#include "sud/sol/solverSudoku.hpp"
#include "sud/sol/simple.hpp"
#include "sud/sol/must.hpp"
#include <array>

namespace sud::sol
{
    class Solver
    {
    private:
        Simple simple;
        Must must;

    public:
        Solver(SolverSudoku &sudoku);

        status_e solve();
    };

}; // namespace sud::sol