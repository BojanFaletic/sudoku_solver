
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <boost/log/trivial.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fmt/core.h>
#include "sudoku.hpp"

using namespace boost;
using namespace std;

namespace sud
{

    Sudoku::Sudoku()
    {
        zero_out_board();
    }

    Sudoku::Sudoku(const string &filename)
    {
        if (read_from_CSV(filename))
        {
            throw std::runtime_error("Invalid sudoku");
        }
    }

    Sudoku::Sudoku(const sudoku_t &board) : board(board)
    {
        if (check())
        {
            throw std::runtime_error("Invalid sudoku");
        }
    }

    void Sudoku::zero_out_board()
    {
        for_each_n(board.begin(), SUDOKU_SIZE, [](auto &row)
                   { fill_n(row.begin(), SUDOKU_SIZE, 0); });
    }

    status_e Sudoku::read_from_CSV(const std::string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            return FILE_NOT_FOUND;
        }

        std::string line;
        square_t row = 0;
        while (getline(file, line))
        {
            if (row >= SUDOKU_SIZE)
            {
                return SUDOKU_INVALID_ROW;
            }

            tokenizer<char_separator<char>> tokenizer(line, char_separator<char>("\t"));
            square_t col = 0;
            for (const auto &token : tokenizer)
            {
                if (col >= SUDOKU_SIZE)
                {
                    return SUDOKU_INVALID_COLUMN;
                }
                if (token == "")
                {
                    continue;
                }
                uint8_t value = stoi(token);
                if (value >= SUDOKU_POSSIBLE_NUMBERS)
                {
                    return SUDOKU_INVALID_VALUE;
                }
                board[row][col] = value;
                col++;
            }

            if (col != SUDOKU_SIZE)
            {
                return SUDOKU_INVALID_COLUMN;
            }
            row++;
        }
        if (check())
        {
            return LOADER_INVALID_PUZZLE;
        }
        return SUCCESS;
    }

    bool Sudoku::is_valid() const
    {
        return check() == SUCCESS;
    }

    status_e Sudoku::check() const
    {
        // check rows
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            std::array<bool, SUDOKU_POSSIBLE_NUMBERS> unique{false};
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                if (board[row][col] != 0)
                {
                    if (unique[board[row][col]])
                    {
                        return SUDOKU_INVALID_ROW;
                    }
                    unique[board[row][col]] = true;
                }
            }
        }

        // check cols
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            std::array<bool, SUDOKU_POSSIBLE_NUMBERS> unique{false};
            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                if (board[row][col] != 0)
                {
                    if (unique[board[row][col]])
                    {
                        return SUDOKU_INVALID_COLUMN;
                    }
                    unique[board[row][col]] = true;
                }
            }
        }

        // check boxes
        for (square_t box_row = 0; box_row < SUDOKU_SIZE; box_row += SUDOKU_BOX_SIZE)
        {
            for (square_t box_col = 0; box_col < SUDOKU_SIZE; box_col += SUDOKU_BOX_SIZE)
            {
                std::array<bool, SUDOKU_POSSIBLE_NUMBERS> unique{false};
                for (square_t r = box_row; r < box_row + SUDOKU_BOX_SIZE; r++)
                {
                    for (square_t c = box_col; c < box_col + SUDOKU_BOX_SIZE; c++)
                    {
                        if (board[r][c] != 0)
                        {
                            if (unique[board[r][c]])
                            {
                                return SUDOKU_INVALID_BOX;
                            }
                            unique[board[r][c]] = true;
                        }
                    }
                }
            }
        }
        return SUCCESS;
    }

    uint8_t Sudoku::count_missing() const
    {
        uint8_t count = 0;
        for (const Point &pt : PointIterator())
        {
            if (board[pt.row][pt.col] == 0)
            {
                count++;
            }
        }
        return count;
    }

    bool Sudoku::is_solved() const
    {
        if (count_missing() != 0)
        {
            return false;
        }
        return check() == SUCCESS;
    }

    status_e Sudoku::save_to_CSV(const string &filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            return FILE_NOT_FOUND;
        }

        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                file << static_cast<int>(board[row][col]);
                if (col != SUDOKU_SIZE - 1)
                {
                    file << '\t';
                }
            }
            if (row != SUDOKU_SIZE - 1)
            {
                file << std::endl;
            }
        }
        file.close();
        return SUCCESS;
    }

    square_t Sudoku::get(const square_t row, const square_t col) const
    {
        return board[row][col];
    }

    square_t Sudoku::get(const Point &point) const
    {
        return board[point.row][point.col];
    }

    void Sudoku::set(const square_t row, const square_t col, const square_t value)
    {
        board[row][col] = value;
    }

    void Sudoku::set(const Point &point, const square_t value)
    {
        board[point.row][point.col] = value;
    }

    square_t Sudoku::operator[](const Point &point) const
    {
        return board[point.row][point.col];
    }

    square_t &Sudoku::operator[](const Point &point)
    {
        return board[point.row][point.col];
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
                if (cell == 0)
                {
                    os << ". ";
                }
                else
                {
                    os << static_cast<int>(cell) << " ";
                }
                j++;
            }
            os << endl;
            i++;
        }
        return os;
    }

} // namespace sud