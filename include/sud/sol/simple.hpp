#pragma once

#include "solver.hpp"

namespace sud::sol
{
    class Simple : public Solver
    {
        friend class SimpleTest;

    private:
        using possible_t = std::array<std::bitset<SUDOKU_POSSIBLE_NUMBERS>, SUDOKU_SIZE>;
        std::array<possible_t, SUDOKU_SIZE> possible_board;

        // search strategy for each of the square
        possible_t row_wise_possible();
        possible_t col_wise_possible();
        possible_t box_wise_possible();

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

        bool is_number_possible(const Point &point, const square_t value) const;
        bool is_number_possible_row(const Point &point, const square_t value) const;
        bool is_number_possible_col(const Point &point, const square_t value) const;
        bool is_number_possible_box(const Point &point, const square_t value) const;

        // used for detecting unique numbers
        bool is_number_possible_outside_box_row(const Point &point, const square_t value) const;
        bool is_number_possible_outside_box_col(const Point &point, const square_t value) const;
        void remove_possible_candidate_inside_box_row(const Point &point, const square_t value);
        void remove_possible_candidate_inside_box_col(const Point &point, const square_t value);

    public:
        Simple(Sudoku &sudoku);

        uint8_t missing_number(const Point &point);

        bool basic_solve();

        // if number is unique in a row/col/box, then it is the only possible number for that square
        void filter_unique();

        void find_possible();
        bool unique_filter();

        void update_possible();

        std::vector<square_t> get_possible(const Point &point) const;

        status_e solve() override;
    };

}; // namespace sud::sol
