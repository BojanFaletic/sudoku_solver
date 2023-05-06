#pragma once

#include "types.hpp"
#include "Sudoku.hpp"
#include <bitset>
#include <array>

namespace sud
{

    class Solver
    {
    protected:
        Sudoku &sudoku;

    public:
        Solver(Sudoku &sudoku);

        virtual status_e solve() = 0;
    };


    class SimpleSolver : public Solver
    {
        friend class SimpleSolverTest; 
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
        

    public:
        SimpleSolver(Sudoku &sudoku);

        void find_possible();
        bool unique_filter();
        
        void update_possible();

        status_e solve() override;
    };

}; // namespace sud