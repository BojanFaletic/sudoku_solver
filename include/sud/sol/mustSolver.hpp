#pragma once

#include "sud/types.hpp"
#include "sud/sudoku.hpp"
#include "sud/sol/common.hpp"

namespace sud::sol
{
    class MustSolver : public Common
    {
    private:        
        // used for detecting unique numbers
        bool is_number_possible_outside_box_row(const Point &point, const Square value) const;
        bool is_number_possible_outside_box_col(const Point &point, const Square value) const;
        void remove_possible_candidate_inside_box_row(const Point &point, const Square value);
        void remove_possible_candidate_inside_box_col(const Point &point, const Square value);

    public:
        MustSolver(Sudoku &sudoku);

        // if number is unique in a row/col/box, then it is the only possible number for that square
        void filter_unique();

        bool solve();
    };

} // namespace sud::sol