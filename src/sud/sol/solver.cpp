#include "sud/sol/solver.hpp"
#include "sud/types.hpp"

namespace sud::sol
{
    Solver::Solver(Sudoku &sudoku) : simple(sudoku), must(sudoku)
    {
    }

} // namespace sud
