#pragma once

#include "sud/types.hpp"
#include "sud/sudoku.hpp"
#include <bitset>
#include <iostream>

namespace sud::sol
{
    using possible_t = std::bitset<SUDOKU_POSSIBLE_NUMBERS>;

    class CustomPossible : public possible_t
    {
    private:
        const Point pt;
        void dbg_write(const possible_t &other) const;

    public:
        using possible_t::possible_t;
        CustomPossible(const Point &pt, const possible_t &possible);

        CustomPossible &operator=(const possible_t &other);
        CustomPossible &operator=(const uint8_t &other);
    };

    class Possible
    {
    private:
        std::array<std::array<CustomPossible, SUDOKU_SIZE>, SUDOKU_SIZE>
            possible_board;
        Sudoku &sudoku;

    public:
        Possible(Sudoku &sudoku);

        CustomPossible operator[](const Point &p);
        CustomPossible operator[](const Point &p) const;
    };

} // namespace sud::sol
