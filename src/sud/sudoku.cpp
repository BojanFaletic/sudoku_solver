
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <boost/log/trivial.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fmt/format.h>
#include "sud/sudoku.hpp"
#include "sud/types.hpp"

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
        std::uint8_t row = 0;
        while (getline(file, line))
        {
            if (row >= SUDOKU_SIZE)
            {
                return SUDOKU_INVALID_ROW;
            }

            tokenizer<char_separator<char>> tokenizer(line, char_separator<char>("\t"));
            std::uint8_t col = 0;
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
                const Square square = stoi(token);
                if (square.to_value() >= SUDOKU_POSSIBLE_NUMBERS)
                {
                    return SUDOKU_INVALID_VALUE;
                }
                board[row][col] = square;
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
        for (std::uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            std::array<bool, SUDOKU_POSSIBLE_NUMBERS> unique{false};
            for (std::uint8_t col = 0; col < SUDOKU_SIZE; col++)
            {
                const Square number = (*this)[{row, col}];
                if (number)
                {
                    if (unique[number.to_value()])
                    {
                        return SUDOKU_INVALID_ROW;
                    }
                    unique[number.to_value()] = true;
                }
            }
        }

        // check cols
        for (std::uint8_t col = 0; col < SUDOKU_SIZE; col++)
        {
            std::array<bool, SUDOKU_POSSIBLE_NUMBERS> unique{false};
            for (std::uint8_t row = 0; row < SUDOKU_SIZE; row++)
            {
                const Square number = (*this)[{row, col}];
                if (number)
                {
                    if (unique[number.to_value()])
                    {
                        return SUDOKU_INVALID_COLUMN;
                    }
                    unique[number.to_value()] = true;
                }
            }
        }

        // check boxes
        for (std::uint8_t box_row = 0; box_row < SUDOKU_SIZE; box_row += SUDOKU_BOX_SIZE)
        {
            for (std::uint8_t box_col = 0; box_col < SUDOKU_SIZE; box_col += SUDOKU_BOX_SIZE)
            {
                std::array<bool, SUDOKU_POSSIBLE_NUMBERS> unique{false};
                for (std::uint8_t r = box_row; r < box_row + SUDOKU_BOX_SIZE; r++)
                {
                    for (std::uint8_t c = box_col; c < box_col + SUDOKU_BOX_SIZE; c++)
                    {
                        const Square number = (*this)[{r, c}];
                        if (number)
                        {
                            if (unique[number.to_value()])
                            {
                                return SUDOKU_INVALID_BOX;
                            }
                            unique[number.to_value()] = true;
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
        PointIterator pt;
        std::for_each(pt.begin(), pt.end(), [&](const Point &pt)
                      { count += (*this)[pt] == Square(0); });
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

    bool Sudoku::is_possible_row(const Point &point, const Square value) const
    {
        for (std::uint8_t col = 0; col < SUDOKU_SIZE; col++)
        {
            if ((*this)[{point.row, col}] == value)
            {
                return false;
            }
        }
        return true;
    }

    bool Sudoku::is_possible_col(const Point &point, const Square value) const
    {
        for (std::uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            if ((*this)[{row, point.col}] == value)
            {
                return false;
            }
        }
        return true;
    }

    bool Sudoku::is_possible_box(const Point &point, const Square value) const
    {
        const Point box = point / SUDOKU_BOX_SIZE * SUDOKU_BOX_SIZE;
        for (std::uint8_t row = box.row; row < box.row + SUDOKU_BOX_SIZE; row++)
        {
            for (std::uint8_t col = box.col; col < box.col + SUDOKU_BOX_SIZE; col++)
            {
                if ((*this)[{row, col}] == value)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool Sudoku::is_possible(const Point &point, const Square value) const
    {
        return is_possible_row(point, value) && is_possible_col(point, value) && is_possible_box(point, value);
    }

    status_e Sudoku::save_to_CSV(const string &filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            return FILE_NOT_FOUND;
        }

        for (std::uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (std::uint8_t col = 0; col < SUDOKU_SIZE; col++)
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

    Square Sudoku::get(const Point &point) const
    {
        return (*this)[point];
    }

    Square Sudoku::set(const Point &point, const Square value)
    {
        return (*this)[point] = value;
    }

    Square Sudoku::operator[](const Point &point) const
    {
        return board[point.row][point.col];
    }

    square_hidden_t Sudoku::operator[](const Point &point)
    {
        return {board[point.row][point.col], point};
    }

    bool Sudoku::operator==(const Sudoku &sudoku) const
    {
        return board == sudoku.board;
    }

    Square square_hidden_t::insert(const Point &point, const Square value)
    {
        std::cout << fmt::format("Inserting {} at ({}, {})\n", value.to_value(), point.row, point.col);
        assertm(!this->value, "Trying to insert a value in a non-empty square");
        this->value = value;
        return value;
        // todo update possible_board
    }

    ostream &operator<<(ostream &os, const Sudoku &sudoku)
    {
        // print sudoku board
        std::uint8_t i = 0;
        for (const auto &row : sudoku.board)
        {
            if ((i % SUDOKU_BOX_SIZE == 0) && (i != 0))
            {
                os << "------+-------+------" << endl;
            }

            std::uint8_t j = 0;
            for (const auto &cell : row)
            {
                if ((j % SUDOKU_BOX_SIZE == 0) && (j != 0))
                {
                    os << "| ";
                }
                if (!cell)
                {
                    os << ". ";
                }
                else
                {
                    os << cell << " ";
                }
                j++;
            }
            os << endl;
            i++;
        }
        return os;
    }

} // namespace sud