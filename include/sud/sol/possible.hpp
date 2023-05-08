#pragma once

#include "sud/types.hpp"
#include "sud/sudoku.hpp"
#include <bitset>

namespace sud::sol
{
    using possible_t = std::bitset<SUDOKU_POSSIBLE_NUMBERS>;

    class Possible
    {
    private:
        std::array<std::array<possible_t, SUDOKU_SIZE>, SUDOKU_SIZE>
            possible_board;
        Sudoku &sudoku;

    public:
        Possible(Sudoku &sudoku);

        possible_t &operator[](const Point &p);
        const possible_t &operator[](const Point &p) const;
    };

} // namespace sud::sol
