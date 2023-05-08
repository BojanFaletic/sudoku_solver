#pragma once

#include "sud/types.hpp"
#include "sud/sol/possible.hpp"
#include "sud/sol/common.hpp"
#include <bitset>
#include <vector>

namespace sud::sol
{
    class Simple : public Common
    {
        friend class SimpleTest;

    private:
        // search strategy for each of the square
        using possible_array_t = std::array<possible_t, SUDOKU_SIZE>;
        
        possible_array_t row_wise_possible();
        possible_array_t col_wise_possible();
        possible_array_t box_wise_possible();

        struct count_t
        {
            uint8_t freq;
            Point pos;
        };

        // filter out unique numbers
        bool unique_filter_row();
        bool unique_filter_col();
        bool unique_filter_box();

        void insert(const Point &point, const square_t value);

        static uint8_t freq_to_value(const std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> &count);
        void update_freq(const Point &point, std::array<count_t, SUDOKU_POSSIBLE_NUMBERS> &count) const;


        
    public:
        Simple(Sudoku &sudoku);

        uint8_t missing_number(const Point &point);

        bool basic_solve();

        void find_possible();
        bool unique_filter();

        void update_possible();


        status_e solve();
    };

}; // namespace sud::sol
