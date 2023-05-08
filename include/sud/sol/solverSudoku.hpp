#pragma once

#include "sud/types.hpp"
#include "sud/sudoku.hpp"
#include <bitset>

namespace sud::sol
{
    using possible_t = std::array<std::bitset<SUDOKU_POSSIBLE_NUMBERS>, SUDOKU_SIZE>;

    class SolverSudoku : public Sudoku
    {
    public:
        std::array<possible_t, SUDOKU_SIZE> possible_board;


    public:

        SolverSudoku(Sudoku &sudoku);
    };

} // namespace sud::sol
