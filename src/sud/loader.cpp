#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include "loader.hpp"

using namespace std;

namespace sud
{

    Error_t sudoku_from_string(const std::string &str, sudoku_t &sudoku);

    Loader::Loader(const string &filename)
    {
        if (load_from_kaggle(filename, numeric_limits<uint32_t>::max()) != LOADER_SUCCESS)
        {
            throw runtime_error("Error loading file");
        }
    }

    Loader::Loader(const std::string &filename, uint32_t max_puzzles)
    {
        if (load_from_kaggle(filename, max_puzzles) != LOADER_SUCCESS)
        {
            throw runtime_error("Error loading file");
        }
    }

    Error_t Loader::load_from_kaggle(const std::string &filename, uint32_t max_puzzles)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            return LOADER_ERROR;
        }

        string line;
        getline(file, line); // skip header
        while (getline(file, line) && data.size() < max_puzzles)
        {
            SudokuData sudoku;
            stringstream ss(line);
            string field;

            // id
            getline(ss, field, ',');
            sudoku.id = stoi(field);

            // puzzle
            getline(ss, field, ',');
            if (sudoku_from_string(field, sudoku.puzzle) != LOADER_SUCCESS)
            {
                return LOADER_INVALID_PUZZLE;
            }

            // solution
            getline(ss, field, ',');
            sudoku_t solution;
            if (sudoku_from_string(field, solution) != LOADER_SUCCESS)
            {
                return LOADER_INVALID_PUZZLE;
            }
            sudoku.solution = solution;

            // clues
            getline(ss, field, ',');
            if (field.size() > 0)
            {
                sudoku.clues = stoi(field);
            }

            // difficulty
            getline(ss, field, ',');
            if (field.size() > 0)
            {
                sudoku.difficulty = stof(field);
            }

            data.push_back(sudoku);
        }
        return LOADER_SUCCESS;
    }

    Error_t sudoku_from_string(const std::string &str, sudoku_t &sudoku)
    {
        if (str.size() != SUDOKU_BOARD_SIZE)
        {
            return LOADER_ERROR;
        }

        // row column
        uint32_t idx = 0;
        for (uint32_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (uint32_t col = 0; col < SUDOKU_SIZE; col++)
            {
                char c = str[idx++];
                if (c == '.')
                {
                    sudoku[row][col] = 0;
                }
                else
                {
                    sudoku[row][col] = c - '0';
                }
            }
        }

        return LOADER_SUCCESS;
    }

    const std::vector<SudokuData> &Loader::get_data() const
    {
        return data;
    }

    const std::optional<SudokuData> Loader::get_data(uint32_t id) const
    {
        auto item = find_if(data.begin(), data.end(), [id](const SudokuData &sudoku)
                            { return sudoku.id == id; });
        if (item == data.end())
        {
            return nullopt;
        }
        return *item;
    }

    const std::optional<sudoku_t> Loader::get_puzzle(uint32_t id) const
    {
        auto item = find_if(data.begin(), data.end(), [id](const SudokuData &sudoku)
                            { return sudoku.id == id; });
        if (item == data.end())
        {
            return nullopt;
        }
        return item->puzzle;
    }

    const std::optional<sudoku_t> Loader::get_solution(uint32_t id) const
    {
        auto item = find_if(data.begin(), data.end(), [id](const SudokuData &sudoku)
                            { return sudoku.id == id; });
        if (item == data.end())
        {
            return nullopt;
        }
        return item->solution;
    }

}; // namespace sud