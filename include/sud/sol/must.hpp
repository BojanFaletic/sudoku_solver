#pragma once

#include "sud/types.hpp"
#include "sud/sudoku.hpp"
#include "sud/sol/Algorithm.hpp"

namespace sud::sol
{
    class Must : public Algorithm
    {
    private:        
        // used for detecting unique numbers
        bool is_number_possible_outside_box_row(const Point &point, const square_t value) const;
        bool is_number_possible_outside_box_col(const Point &point, const square_t value) const;
        void remove_possible_candidate_inside_box_row(const Point &point, const square_t value);
        void remove_possible_candidate_inside_box_col(const Point &point, const square_t value);

    public:
        Must(Sudoku &sudoku);

        // if number is unique in a row/col/box, then it is the only possible number for that square
        void filter_unique();


        bool solve();
    };

} // namespace sud::sol