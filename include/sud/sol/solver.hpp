#pragma once

#include "sud/types.hpp"
#include "sud/sol/possible.hpp"
#include "sud/sol/simpleSolver.hpp"
#include "sud/sol/mustSolver.hpp"
#include <array>

namespace sud::sol
{
    class Solver
    {
    private:
        SimpleSolver simple;
        MustSolver must;

    public:
        Solver(Sudoku &sudoku);

        status_e solve();
    };

}; // namespace sud::sol