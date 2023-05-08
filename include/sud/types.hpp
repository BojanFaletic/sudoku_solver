#pragma once

#include "sud/square.hpp"
#include <cstdint>
#include <array>
#include <set>
#include <cassert>
#include <optional>
#include <fmt/format.h>

#define assertm(exp, msg) assert(((void)msg, exp))

namespace sud
{

    /*-----------------*/
    /* -- constants -- */
    /* -----------------*/

    // size of the sudoku board
    constexpr size_t SUDOKU_SIZE = 9;

    // size of the sudoku box
    constexpr size_t SUDOKU_BOX_SIZE = 3;

    // size of the sudoku board
    constexpr size_t SUDOKU_BOARD_SIZE = SUDOKU_SIZE * SUDOKU_SIZE;

    // number of numbers in the sudoku board
    constexpr size_t SUDOKU_POSSIBLE_NUMBERS = 10;

    /* -----------------*/
    /* -- enumerators -- */
    /* -----------------*/

    enum status_e
    {
        SUCCESS = 0,
        ERROR,
        SUDOKU_SUCCESS,
        SUDOKU_ERROR,
        LOADER_SUCCESS,
        LOADER_ERROR,
        LOADER_INVALID_PUZZLE,
        SUDOKU_NOT_SOLVED,
        SUDOKU_SOLVED,
        SUDOKU_INVALID_ROW,
        SUDOKU_INVALID_COLUMN,
        SUDOKU_INVALID_BOX,
        SUDOKU_INVALID_VALUE,
        FILE_NOT_FOUND,
    };

    /*-----------------*/
    /* -- typedefs -- */
    /* -----------------*/

    // description of the sudoku board
    using sudoku_t = std::array<std::array<Square, SUDOKU_SIZE>, SUDOKU_SIZE>;

    struct Point
    {
        std::uint8_t row;
        std::uint8_t col;

        Point() : row(0), col(0) {}
        Point(std::uint8_t row, std::uint8_t col) : row(row), col(col) {}

        // operators
        Point operator*(const std::uint8_t &value) const;
        Point operator/(const std::uint8_t &value) const;
        Point operator+(const Point &other) const;
        bool operator==(const Point &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Point &point);
    };

    class PointIterator
    {
    private:
        Point point;

    public:
        PointIterator() : point({0, 0}) {}
        PointIterator(const Point &point) : point(point) {}

        PointIterator &operator++();
        bool operator!=(const PointIterator &other) const;
        Point operator*() const;

        PointIterator begin() const;
        PointIterator end() const;
    };

    struct SudokuData
    {
        uint32_t id;
        sudoku_t puzzle;
        std::optional<sudoku_t> solution;
        std::optional<uint32_t> clues;
        std::optional<float> difficulty;
    };

}; // namespace sud

template <>
struct fmt::formatter<sud::Point>
{
    constexpr auto parse(format_parse_context &ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const sud::Point &p, FormatContext &ctx)
    {
        return format_to(ctx.out(), "({}, {})", p.row, p.col);
    }
};
