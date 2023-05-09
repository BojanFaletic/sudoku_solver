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
        bool is_number_possible_row(const Point &point, const Square value) const;
        bool is_number_possible_col(const Point &point, const Square value) const;
        bool is_number_possible_box(const Point &point, const Square value) const;

        // search strategy for each of the square
        using possible_array_t = std::array<CustomPossible, SUDOKU_SIZE>;

        possible_array_t row_wise_possible();
        possible_array_t col_wise_possible();
        possible_array_t box_wise_possible();

    public:
        Possible possible;
        Sudoku &sudoku;

        Common(Sudoku &sudoku);

        void find_possible();
        bool basic_solve();

        uint8_t missing_number(const Point &point);
        bool is_number_possible(const Point &point, const Square value) const;
        std::vector<Square> get_possible(const Point &point) const;
    };

}