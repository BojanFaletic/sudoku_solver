#pragma once

#include <string>
#include <vector>
#include <optional>
#include "sudoku.hpp"

namespace sud
{
    class Loader
    {
    private:
        std::vector<SudokuData> data;

        Error_t load_from_kaggle(const std::string &filename);
    public:
        Loader(const std::string &filename);

        const std::vector<SudokuData> &get_data() const;

        const std::optional<SudokuData> &get_data(uint32_t id) const;

        const std::optional<sudoku_t> &get_puzzle(uint32_t id) const;
        const std::optional<sudoku_t> &get_solution(uint32_t id) const;
    };
};