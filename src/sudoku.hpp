#pragma once

#include <errno.h>
#include <array>
#include <cstdint>
#include <ostream>
#include <stdbool.h>
#include <set>
#include <boost/container/static_vector.hpp>

namespace sud
{
    using namespace std;
    using boost::container::static_vector;
    /*-----------------*/
    /* -- constants -- */
    /* -----------------*/

    // size of the sudoku board
    const size_t SUDOKU_SIZE = 9;

    // size of the sudoku box
    const size_t SUDOKU_BOX_SIZE = 3;

    // number of numbers in the sudoku board
    const size_t SUDOKU_POSSIBLE_NUMBERS = 10;

    /* Error codes */
    const int SUDOKU_ERROR = -1;
    const int SUDOKU_OK = 0;

    /*-----------------*/
    /* -- typedefs -- */
    /* -----------------*/

    // type of a square in the sudoku board
    using square_t = uint8_t;

    // description of the sudoku board
    using sudoku_t = array<array<uint8_t, SUDOKU_SIZE>, SUDOKU_SIZE>;

    // boolean array of size 9 indicating which numbers are missing
    using missing_t = set<square_t>;

    // boolean array of size 9x9 indicating which numbers are missing
    using missing_full_t = array<array<missing_t, SUDOKU_SIZE>, SUDOKU_SIZE>;

    struct Point
    {
        square_t row;
        square_t col;
    };

    using solve_candidates_t = std::array<std::array<missing_t, SUDOKU_SIZE>, SUDOKU_SIZE>;

    /*-----------------*/
    /* -- functions -- */
    /* -----------------*/
    class Sudoku
    {
    private:
        sudoku_t board;

        void load_from_CSV(const string filename);
        void load_from_kaggle(const string filename);

        std::array<missing_t, SUDOKU_SIZE> possible_items_in_row(const square_t row) const;
        std::array<missing_t, SUDOKU_SIZE> possible_items_in_col(const square_t col) const;
        std::array<missing_t, SUDOKU_SIZE> possible_items_in_box(const square_t row, const square_t col) const;

        // Solver techniques
        bool row_solver();
        bool col_solver();
        bool box_solver();

    public:
        Sudoku();
        Sudoku(const string filename);

        bool save_to_CSV(const string filename);

        bool solve();
        bool check();

        square_t get(const square_t row, const square_t col) const;
        void set(const square_t row, const square_t col, const square_t value);
        missing_t possible_items(const square_t row, const square_t col) const;

        friend ostream &operator<<(ostream &os, const Sudoku &sudoku);
    };

}; // namespace sud
