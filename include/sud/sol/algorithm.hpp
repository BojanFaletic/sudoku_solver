#pragma once

#include "sud/types.hpp"
#include "sud/sol/solverSudoku.hpp"
#include <vector>

namespace sud::sol
{
    class Algorithm
    {
    private:
        // possible numbers for each square
        SolverSudoku &sudoku;

        bool is_number_possible_row(const Point &point, const square_t value) const;
        bool is_number_possible_col(const Point &point, const square_t value) const;
        bool is_number_possible_box(const Point &point, const square_t value) const;

    public:
        Algorithm(SolverSudoku &sudoku);

        bool is_number_possible(const Point &point, const square_t value) const;
        std::vector<square_t> get_possible(const Point &point) const;
    };
}