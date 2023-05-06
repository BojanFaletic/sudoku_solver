#pragma once

#include <string>
#include "types.hpp"

namespace sud
{
    class Sudoku
    {
    private:
        sudoku_t board;

        void zero_out_board();
        status_e read_from_CSV(const std::string &filename);

    public:
        Sudoku();
        Sudoku(const std::string &filename);
        Sudoku(const sudoku_t &board);

        // Save the sudoku to a CSV file
        status_e save_to_CSV(const std::string &filename);

        // Check if the sudoku is solved
        status_e check();

        // Getters and setters
        square_t get(const square_t row, const square_t col) const;
        square_t get(const Point &point) const;

        void set(const square_t row, const square_t col, const square_t value);
        void set(const Point &point, const square_t value);

        bool is_solved() const;

        // Print the sudoku board
        square_t operator[](const Point &point) const;
        square_t &operator[](const Point &point);

        friend std::ostream &operator<<(std::ostream &os, const Sudoku &sudoku);
    };

}; // namespace sud
