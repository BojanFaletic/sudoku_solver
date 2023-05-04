#pragma once

#include <vector>
#include "types.hpp"

namespace sud
{
    class Loader
    {
    private:
        std::vector<SudokuData> data;

        status_e load_from_kaggle(const std::string &filename, uint32_t max_puzzles);
    public:
        Loader(const std::string &filename);
        Loader(const std::string &filename, uint32_t max_puzzles);


        const std::vector<SudokuData> &get_data() const;

        const std::optional<SudokuData> get_data(uint32_t id) const;

        const std::optional<sudoku_t> get_puzzle(uint32_t id) const;
        const std::optional<sudoku_t> get_solution(uint32_t id) const;
    };
}; // namespace sud