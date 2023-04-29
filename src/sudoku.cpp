#include "sudoku.hpp"

#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

namespace sud
{

    void init_board(sudoku_t &board)
    {
        for (auto &row : board)
        {
            for (auto &cell : row)
            {
                cell = 0;
            }
        }
    }

    unique_t missing_items_X_dir(sudoku_t &sud, square_t row)
    {
        unique_t result{false};
        for_each(sud[row].begin(), sud[row].end(), [&result](uint8_t &item)
                 { result[item] |= true; });
        return result;
    }

    unique_t missing_items_Y_dir(sudoku_t &sud, square_t col)
    {
        unique_t result{false};
        for_each(sud.begin(), sud.end(), [&result, col](const array<uint8_t, SUDOKU_SIZE> &row)
                 { result[row[col]] |= true; });
        return result;
    }

    unique_t missing_items_box(sudoku_t &sud, square_t row, square_t col)
    {
        unique_t result{false};
        const square_t box_row = (row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        const square_t box_col = (col / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        for (square_t r = box_row; r < box_row + SUDOKU_BOX_SIZE; r++)
        {
            for (square_t c = box_col; c < box_col + SUDOKU_BOX_SIZE; c++)
            {
                result[sud[r][c]] |= true;
            }
        }
        return result;
    }

    missing_t possible_items(sudoku_t &sud, square_t row, square_t col)
    {
        const unique_t missing_X = missing_items_X_dir(sud, row);
        const unique_t missing_Y = missing_items_Y_dir(sud, col);
        const unique_t missing_box = missing_items_box(sud, row, col);
        missing_t result;
        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            result[i - 1] = !(missing_X[i] || missing_Y[i] || missing_box[i]);
        }
        return result;
    }

    missing_full_t possible_items_full(sudoku_t &sud)
    {
        missing_full_t result;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                result[row][col] = possible_items(sud, row, col);
            }
        }
        return result;
    }

    bool Sudoku::load_from_CSV(const string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            return false;
        }
        string line;
        square_t row = 0;
        while (getline(file, line))
        {
            square_t col = 0;
            stringstream ss(line);
            string cell;
            while (getline(ss, cell, '	'))
            {
                board[row][col] = stoi(cell);
                col++;
            }
            row++;
        }
        return true;
    }


    Sudoku::Sudoku()
    {
        init_board(board);
    }

    ostream &operator<<(ostream &os, const Sudoku &sudoku)
    {
        // print sudoku board
        square_t i = 0;
        for (const auto &row : sudoku.board)
        {
            if ((i % SUDOKU_BOX_SIZE == 0) && (i != 0))
            {
                os << "------+-------+------" << endl;
            }

            square_t j = 0;
            for (const auto &cell : row)
            {
                if ((j % SUDOKU_BOX_SIZE == 0) && (j != 0))
                {
                    os << "| ";
                }
                os << static_cast<int>(cell) << " ";
                j++;
            }
            os << endl;
            i++;
        }
        return os;
    }

} // namespace sud