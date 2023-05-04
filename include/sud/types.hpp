#pragma once

#include <stdint.h>
#include <optional>
#include <array>
#include <set>

namespace sud{

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

    // type of a square in the sudoku board
    using square_t = uint8_t;

    // description of the sudoku board
    using sudoku_t = std::array<std::array<uint8_t, SUDOKU_SIZE>, SUDOKU_SIZE>;

    // boolean array of size 9 indicating which numbers are missing
    using missing_t = std::set<square_t>;

    // boolean array of size 9x9 indicating which numbers are missing
    using missing_full_t = std::array<std::array<missing_t, SUDOKU_SIZE>, SUDOKU_SIZE>;

    struct Point
    {
        square_t row;
        square_t col;
    };

    struct SudokuData
    {
        uint32_t id;
        sudoku_t puzzle;
        std::optional<sudoku_t> solution;
        std::optional<uint32_t> clues;
        std::optional<float> difficulty;
    };

    using solve_candidates_t = std::array<std::array<missing_t, SUDOKU_SIZE>, SUDOKU_SIZE>;
}; // namespace sud