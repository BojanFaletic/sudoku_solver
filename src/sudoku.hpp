#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <stdbool.h>

namespace sud
{
    using namespace std;
    /*-----------------*/
    /* -- constants -- */
    /* -----------------*/

    // size of the sudoku board
    const size_t SUDOKU_SIZE = 9;

    // size of the sudoku box
    const size_t SUDOKU_BOX_SIZE = 3;

    // number of numbers in the sudoku board
    const size_t SUDOKU_POSSIBLE_NUMBERS = 10;

    /*-----------------*/
    /* -- typedefs -- */
    /* -----------------*/

    // type of a square in the sudoku board
    using square_t = uint8_t;

    // description of the sudoku board
    using sudoku_t = array<array<uint8_t, SUDOKU_SIZE>, SUDOKU_SIZE>;

    // boolean array of size 10 indicating which numbers are missing
    using unique_t = array<bool, SUDOKU_POSSIBLE_NUMBERS>;

    // boolean array of size 9 indicating which numbers are missing
    using missing_t = array<bool, SUDOKU_POSSIBLE_NUMBERS - 1>;

    // boolean array of size 9x9 indicating which numbers are missing
    using missing_full_t = array<array<missing_t, SUDOKU_SIZE>, SUDOKU_SIZE>;

    void init_board(sudoku_t &board);
    missing_t possible_items(sudoku_t &sud, square_t row, square_t col);
    missing_full_t possible_items_full(sudoku_t &sud);

    class Sudoku
    {
    private:
        sudoku_t board;

    public:
        Sudoku();

        bool load_from_CSV(const string filename);
        friend ostream &operator<<(ostream &os, const Sudoku &sudoku);
    };

}; // namespace sud
