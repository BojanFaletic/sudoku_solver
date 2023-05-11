#pragma once

#include <string>
#include "sud/types.hpp"

namespace sud
{
    class square_hidden_t
    {
    private:
        Square &value;
        const Point &point;

    public:
        square_hidden_t(Square &value, const Point &point) : value(value), point(point) {}

        Square insert(const Point &point, const Square value);

        Square operator=(const Square value)
        {
            insert(point, value);
            return value;
        }

        std::uint8_t to_value() const
        {
            return value.to_value();
        }

        operator Square() const
        {
            return value;
        }

        operator bool() const
        {
            return value.to_value() == 0;
        }
    };

    class Sudoku
    {
    private:
        sudoku_t board;

        void zero_out_board();
        status_e read_from_CSV(const std::string &filename);
        uint8_t count_missing() const;

        bool is_possible_row(const Point &point, const Square value) const;
        bool is_possible_col(const Point &point, const Square value) const;
        bool is_possible_box(const Point &point, const Square value) const;

    public:
        Sudoku();
        Sudoku(const std::string &filename);
        Sudoku(const sudoku_t &board);

        // Save the sudoku to a CSV file
        status_e save_to_CSV(const std::string &filename);

        // Check if the sudoku is solved
        status_e check() const;

        // Getters and setters
        Square get(const Point &point) const;
        Square set(const Point &point, const Square value);

        bool is_solved() const;
        bool is_valid() const;
        bool is_possible(const Point &point, const Square value) const;

        // Print the sudoku board
        friend std::ostream &operator<<(std::ostream &os, const Sudoku &sudoku);

        // for reading and writing to the sudoku
        bool operator==(const Sudoku &sudoku) const;
        square_hidden_t operator[](const Point &point);
        Square operator[](const Point &point) const;
    };

}; // namespace sud
