#pragma once

#include "sud/types.hpp"
#include "sud/sol/possible.hpp"
#include "sud/sudoku.hpp"
#include <vector>

namespace sud::sol
{
    class Common
    {
    private:
        // possible numbers for each square
        bool is_number_possible_row(const Point &point, const square_t value) const;
        bool is_number_possible_col(const Point &point, const square_t value) const;
        bool is_number_possible_box(const Point &point, const square_t value) const;

    public:
        Possible possible;
        Sudoku &sudoku;

        Common(Sudoku &sudoku);

        void insert(const Point &point, const square_t value);

        uint8_t missing_number(const Point &point);

        bool is_number_possible(const Point &point, const square_t value) const;
        std::vector<square_t> get_possible(const Point &point) const;
    };
}