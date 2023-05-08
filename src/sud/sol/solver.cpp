#include "sud/sol/solver.hpp"
#include "sud/types.hpp"
#include <algorithm>
#include <cmath>
#include <cassert>
#include <fmt/format.h>
#include <iostream>

namespace sud::sol
{
    Solver::Solver(SolverSudoku &sudoku) : simple(sudoku), must(sudoku)
    {
    }

} // namespace sud
