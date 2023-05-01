#include "sudoku.hpp"

#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <boost/tokenizer.hpp>
#include <iostream>
#include <map>

#include <fmt/core.h>

using namespace boost;

namespace sud
{

    Sudoku::Sudoku()
    {
        for (auto &row : board)
        {
            for (auto &cell : row)
            {
                cell = 0;
            }
        }
    }

    Sudoku::Sudoku(const string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file");
        }
        std::string line;
        square_t row = 0;
        while (getline(file, line))
        {
            if (row >= SUDOKU_SIZE)
            {
                throw std::runtime_error("Too many rows");
            }

            tokenizer<char_separator<char>> tokenizer(line, char_separator<char>("\t"));
            square_t col = 0;
            for (const auto &token : tokenizer)
            {
                if (col >= SUDOKU_SIZE)
                {
                    throw std::runtime_error("Too many columns");
                }
                if (token == "")
                {
                    continue;
                }
                uint8_t value = stoi(token);
                if (value >= SUDOKU_POSSIBLE_NUMBERS)
                {
                    throw std::runtime_error("Invalid value");
                }
                board[row][col] = value;
                col++;
            }

            if (col != SUDOKU_SIZE)
            {
                throw std::runtime_error("Too few columns");
            }
            row++;
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

    missing_t Sudoku::possible_items(const square_t row, const square_t col)
    {
        const unique_t missing_X = missing_items_X_dir(board, row);
        const unique_t missing_Y = missing_items_Y_dir(board, col);
        const unique_t missing_box = missing_items_box(board, row, col);
        missing_t result;
        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (!(missing_X[i] || missing_Y[i] || missing_box[i]))
            {
                result.push_back(i);
            }
        }
        return result;
    }

    bool Sudoku::solve()
    {
        solve_candidates_t solve_candidates;
        uint8_t n_unsolved = 0;
        // find missing items
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                if (board[row][col] == 0)
                {
                    solve_candidates[row][col]= possible_items(row, col);
                    n_unsolved++;
                }
            }
        }

        // try to solve
        uint8_t n_unsolved_prev = 0;
        while (n_unsolved > 0 && n_unsolved != n_unsolved_prev)
        {
            n_unsolved_prev = n_unsolved;
            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                for (square_t col = 0; col < SUDOKU_SIZE; col++)
                {
                    if (board[row][col] == 0)
                    {
                        const missing_t &missing = solve_candidates[row][col];
                        if (missing.size() == 1)
                        {
                            std::cout << fmt::format("Solved {} {} = {}\n", row, col, missing[0]);

                            board[row][col] = missing[0];
                            solve_candidates[row][col].clear();
                            n_unsolved--;
                        }
                    }
                }
            }
        }
        return n_unsolved == 0;
    }

    missing_t to_number(const unique_t &unique)
    {
        missing_t result;
        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (unique[i])
            {
                result.push_back(i);
            }
        }
        return result;
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