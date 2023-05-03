
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
        for (auto &row : board)
        {
            for (auto &cell : row)
            {
                cell = 0;
            }
        }
    }

    Sudoku::Sudoku(const string &filename)
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
        if (!check())
        {
            throw std::runtime_error("Invalid sudoku");
        }
    }

    Sudoku::Sudoku(const sudoku_t &board) : board(board)
    {
        if (!check())
        {
            throw std::runtime_error("Invalid sudoku");
        }
    }

    missing_t Sudoku::possible_items(const square_t row, const square_t col) const
    {
        std::set<square_t> result = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        // check row
        for (square_t c = 0; c < SUDOKU_SIZE; c++)
        {
            result.erase(board[row][c]);
        }

        // check column
        for (square_t r = 0; r < SUDOKU_SIZE; r++)
        {
            result.erase(board[r][col]);
        }

        // check box
        const square_t box_row = (row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        const square_t box_col = (col / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        for (square_t r = box_row; r < box_row + SUDOKU_BOX_SIZE; r++)
        {
            for (square_t c = box_col; c < box_col + SUDOKU_BOX_SIZE; c++)
            {
                result.erase(board[r][c]);
            }
        }
        return result;
    }

    void erase_missing_Y_dir(missing_full_t &missing, square_t col, uint8_t value)
    {
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            missing[row][col].erase(value);
        }
    }

    void erase_missing_X_dir(missing_full_t &missing, square_t row, uint8_t value)
    {
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            auto el = std::find(missing[row][col].begin(), missing[row][col].end(), value);
            if (el != missing[row][col].end())
            {
                missing[row][col].erase(el);
            }
        }
    }

    void erase_missing_box(missing_full_t &missing, square_t row, square_t col, uint8_t value)
    {
        const square_t box_row = (row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        const square_t box_col = (col / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        for (square_t r = box_row; r < box_row + SUDOKU_BOX_SIZE; r++)
        {
            for (square_t c = box_col; c < box_col + SUDOKU_BOX_SIZE; c++)
            {
                missing[r][c].erase(value);
            }
        }
    }

    bool solve_X_dir(sudoku_t &sud, square_t row, missing_full_t &missing)
    {
        std::array<std::pair<uint8_t, square_t>, SUDOKU_POSSIBLE_NUMBERS> hist;
        for_each(hist.begin(), hist.end(), [](std::pair<uint8_t, square_t> &item)
                 { item.first = 0; });

        bool result = false;
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            if (sud[row][col] == 0)
            {
                for (const auto &item : missing[row][col])
                {
                    hist[item].first++;
                    hist[item].second = col;
                }
            }
        }

        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (hist[i].first == 1)
            {
                BOOST_LOG_TRIVIAL(trace) << fmt::format("Inserting {} in ({}, {})\n", i, row, hist[i].second);
                sud[row][hist[i].second] = i;
                missing[row][hist[i].second].clear();
                erase_missing_X_dir(missing, row, i);
                result = true;
            }
        }
        return result;
    }

    bool solve_Y_dir(sudoku_t &sud, square_t col, missing_full_t &missing)
    {
        std::array<std::pair<uint8_t, square_t>, SUDOKU_POSSIBLE_NUMBERS> hist;
        for_each(hist.begin(), hist.end(), [](std::pair<uint8_t, square_t> &item)
                 { item.first = 0; });

        bool result = false;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            if (sud[row][col] == 0)
            {
                for (const auto &item : missing[row][col])
                {
                    hist[item].first++;
                    hist[item].second = row;
                }
            }
        }

        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (hist[i].first == 1)
            {
                BOOST_LOG_TRIVIAL(trace) << fmt::format("Inserting {} in ({}, {})\n", i, hist[i].second, col);
                sud[hist[i].second][col] = i;
                missing[hist[i].second][col].clear();
                erase_missing_Y_dir(missing, col, i);
                result = true;
            }
        }
        return result;
    }

    bool solve_box(sudoku_t &sud, square_t row, square_t col, missing_full_t &missing)
    {
        std::array<std::pair<uint8_t, Point>, SUDOKU_POSSIBLE_NUMBERS> hist;
        for_each(hist.begin(), hist.end(), [](std::pair<uint8_t, Point> &item)
                 { item.first = 0; });

        bool result = false;
        const square_t box_row = (row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        const square_t box_col = (col / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;

        for (square_t r = box_row; r < box_row + SUDOKU_BOX_SIZE; r++)
        {
            for (square_t c = box_col; c < box_col + SUDOKU_BOX_SIZE; c++)
            {
                if (sud[r][c] == 0)
                {
                    for (const auto &item : missing[r][c])
                    {
                        hist[item].first++;
                        hist[item].second = Point(r, c);
                    }
                }
            }
        }

        for (uint8_t i = 1; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (hist[i].first == 1)
            {
                BOOST_LOG_TRIVIAL(trace) << fmt::format("Inserting {} in ({}, {})\n", i, hist[i].second.row, hist[i].second.col);
                sud[hist[i].second.row][hist[i].second.col] = i;
                missing[hist[i].second.row][hist[i].second.col].clear();
                erase_missing_box(missing, hist[i].second.row, hist[i].second.col, i);
                result = true;
            }
        }
        return result;
    }

    missing_t Sudoku::possible_items_in_row(square_t row) const
    {
        missing_t result = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            result.erase(get(row, col));
        }
        return result;
    }

    missing_t Sudoku::possible_items_in_col(square_t col) const
    {
        missing_t result = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            result.erase(get(row, col));
        }
        return result;
    }

    missing_t Sudoku::possible_items_in_box(square_t row, square_t col) const
    {
        missing_t result = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        const square_t box_row = (row / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        const square_t box_col = (col / SUDOKU_BOX_SIZE) * SUDOKU_BOX_SIZE;
        for (square_t r = box_row; r < box_row + SUDOKU_BOX_SIZE; r++)
        {
            for (square_t c = box_col; c < box_col + SUDOKU_BOX_SIZE; c++)
            {
                result.erase(get(r, c));
            }
        }
        return result;
    }

    std::array<missing_t, SUDOKU_SIZE> Sudoku::possible_items_in_row() const
    {
        std::array<missing_t, SUDOKU_SIZE> result;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            result[row] = possible_items_in_row(row);
        }
        return result;
    }

    std::array<missing_t, SUDOKU_SIZE> Sudoku::possible_items_in_col() const
    {
        std::array<missing_t, SUDOKU_SIZE> result;
        for (square_t col = 0; col < SUDOKU_SIZE; col++)
        {
            result[col] = possible_items_in_col(col);
        }
        return result;
    }

    std::array<missing_t, SUDOKU_SIZE> Sudoku::possible_items_in_box() const
    {
        std::array<missing_t, SUDOKU_SIZE> result;
        for (size_t idx = 0; idx < SUDOKU_SIZE; idx++)
        {
            result[idx] = possible_items_in_box(idx / SUDOKU_BOX_SIZE, idx % SUDOKU_BOX_SIZE);
        }
        return result;
    }

    solve_candidates_t Sudoku::check_missing() const
    {
        solve_candidates_t solve_candidates;
        const std::array<missing_t, SUDOKU_SIZE> missing_row = possible_items_in_row();
        const std::array<missing_t, SUDOKU_SIZE> missing_col = possible_items_in_col();
        const std::array<missing_t, SUDOKU_SIZE> missing_box = possible_items_in_box();

        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                if (get(row, col) == 0)
                {
                    // TODO: make this more efficient
                    solve_candidates[row][col] = possible_items(row, col);
                }
                else
                {
                    solve_candidates[row][col] = {};
                }
            }
        }
        return solve_candidates;
    }

    uint8_t count_missing(const Sudoku &sud)
    {
        uint8_t n_unsolved = 0;
        for (square_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                if (sud.get(row, col) == 0)
                {
                    n_unsolved++;
                }
            }
        }
        return n_unsolved;
    }

    bool Sudoku::row_solver()
    {
        uint8_t n_unsolved_old = 0;
        uint8_t n_unsolved = count_missing(*this);
        uint8_t n_unsolved_origin = n_unsolved;

        while (n_unsolved_old != n_unsolved)
        {
            n_unsolved_old = n_unsolved;
            solve_candidates_t solve_candidates = check_missing();

            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                while (solve_X_dir(board, row, solve_candidates))
                {
                    n_unsolved--;
                }
            }
        }
        return n_unsolved_origin < n_unsolved;
    }

    bool Sudoku::col_solver()
    {
        uint8_t n_unsolved_old = 0;
        uint8_t n_unsolved = count_missing(*this);
        uint8_t n_unsolved_origin = n_unsolved;

        while (n_unsolved_old != n_unsolved)
        {
            n_unsolved_old = n_unsolved;
            solve_candidates_t solve_candidates = check_missing();

            for (square_t col = 0; col < SUDOKU_SIZE; col++)
            {
                while (solve_Y_dir(board, col, solve_candidates))
                {
                    n_unsolved--;
                }
            }
        }
        return n_unsolved_origin < n_unsolved;
    }

    bool Sudoku::box_solver()
    {
        uint8_t n_unsolved_old = 0;
        uint8_t n_unsolved = count_missing(*this);
        const uint8_t n_unsolved_origin = n_unsolved;

        while (n_unsolved_old != n_unsolved)
        {
            n_unsolved_old = n_unsolved;
            solve_candidates_t solve_candidates = check_missing();

            for (square_t row = 0; row < SUDOKU_SIZE; row++)
            {
                for (square_t col = 0; col < SUDOKU_SIZE; col++)
                {
                    while (solve_box(board, row, col, solve_candidates))
                    {
                        n_unsolved--;
                    }
                }
            }
        }
        return n_unsolved_origin < n_unsolved;
    }

    bool Sudoku::solve()
    {
        while (row_solver() || col_solver() || box_solver())
            ;
        return count_missing(*this) == 0;
    }

    bool Sudoku::check()
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
                        return false;
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
                        return false;
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
                                return false;
                            }
                            unique[board[r][c]] = true;
                        }
                    }
                }
            }
        }
        return true;
    }

    bool Sudoku::save_to_CSV(const string &filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            return false;
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
        return true;
    }

    square_t Sudoku::get(const square_t row, const square_t col) const
    {
        return board[row][col];
    }

    void Sudoku::set(const square_t row, const square_t col, const square_t value)
    {
        board[row][col] = value;
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