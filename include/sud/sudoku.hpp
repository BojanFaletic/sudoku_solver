#pragma once

#include <string>
#include "sud/types.hpp"

namespace sud
{
    struct square_hidden_t
    {
        square_t &value;
        const Point &point;

        square_t insert(const Point &point, const square_t value);

        square_t operator=(const square_t value)
        {
            insert(point, value);
            return value;
        }

        operator square_t() const
        {
            return value;
        }
    };

    class Sudoku
    {
    private:
        sudoku_t board;

        void zero_out_board();
        status_e read_from_CSV(const std::string &filename);
        uint8_t count_missing() const;

        bool is_possible_row(const Point &point, const square_t value) const;
        bool is_possible_col(const Point &point, const square_t value) const;
        bool is_possible_box(const Point &point, const square_t value) const;

    public:
        Sudoku();
        Sudoku(const std::string &filename);
        Sudoku(const sudoku_t &board);

        // Save the sudoku to a CSV file
        status_e save_to_CSV(const std::string &filename);

        // Check if the sudoku is solved
        status_e check() const;

        // Getters and setters
        square_t get(const square_t row, const square_t col) const;
        square_t get(const Point &point) const;

        void set(const square_t row, const square_t col, const square_t value);
        void set(const Point &point, const square_t value);

        bool is_solved() const;
        bool is_valid() const;
        bool is_possible(const Point &point, const square_t value) const;

        // Print the sudoku board
        friend std::ostream &operator<<(std::ostream &os, const Sudoku &sudoku);

        // for reading and writing to the sudoku
        square_hidden_t operator[](const Point &point);
        square_t operator[](const Point &point) const;
    };

}; // namespace sud
