#pragma once

#include "types.hpp"
#include "sudoku.hpp"

namespace sud::sol
{
    class Must
    {
    private:
        Sudoku &sudoku;

    public:
        Must(Sudoku &sudoku);

        bool solve();
    };

} // namespace sud::sol