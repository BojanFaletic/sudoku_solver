#pragma once

#include <string>
#include "types.hpp"

namespace sud
{
    class Sudoku
    {
    private:
        sudoku_t board;

        // Types of loading CSV files
        void load_from_CSV(const std::string filename);

        // Get candidates for a given square in a row, col or box
        missing_t possible_items_in_row(square_t row) const;
        missing_t possible_items_in_col(square_t col) const;
        missing_t possible_items_in_box(square_t row, square_t col) const;

        // Get candidates for a full row, col or box
        std::array<missing_t, SUDOKU_SIZE> possible_items_in_row() const;
        std::array<missing_t, SUDOKU_SIZE> possible_items_in_col() const;
        std::array<missing_t, SUDOKU_SIZE> possible_items_in_box() const;

        // Solver techniques
        bool row_solver();
        bool col_solver();
        bool box_solver();

    public:
        Sudoku();
        Sudoku(const std::string &filename);
        Sudoku(const sudoku_t &board);

        // Save the sudoku to a CSV file
        bool save_to_CSV(const std::string &filename);

        // Solve the sudoku
        bool solve();

        // Check if the sudoku is solved
        bool check();

        // Get the possible items in a given square
        missing_t possible_items(const square_t row, const square_t col) const;

        // Get possible items for all squares
        solve_candidates_t check_missing() const;

        // Getters and setters
        square_t get(const square_t row, const square_t col) const;
        void set(const square_t row, const square_t col, const square_t value);

        // Print the sudoku board
        friend std::ostream &operator<<(std::ostream &os, const Sudoku &sudoku);
    };

}; // namespace sud
