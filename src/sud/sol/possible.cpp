#include "sud/sol/possible.hpp"

namespace sud::sol
{
    /* ------------------------------------------------ */
    /* ------------------ Possible ------------------- */
    /* ------------------------------------------------ */

    Possible::Possible(Sudoku &sudoku) : sudoku(sudoku)
    {
    }

    CustomPossible Possible::operator[](const Point &p)
    {
        return {p, possible_board[p.row][p.col]};
    }

    CustomPossible Possible::operator[](const Point &p) const
    {
        return {p, possible_board[p.row][p.col]};
    }



    /* ------------------------------------------------ */
    /* ---------------- CustomPossible ---------------- */
    /* ------------------------------------------------ */

    CustomPossible::CustomPossible(const Point &pt, const possible_t &possible) : possible_t(possible), pt(pt)
    {
    }

    void CustomPossible::dbg_write(const possible_t &other) const
    {
        std::set<uint8_t> prev;
        std::set<uint8_t> other_set;

        for (uint8_t i = 0; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (this->test(i))
            {
                prev.insert(i);
            }
            if (other.test(i))
            {
                other_set.insert(i);
            }
        }

        std::cout << fmt::format("Possible {} : {} --> {}\n",
                                    pt, fmt::join(prev, ", "), fmt::join(other_set, ", "));
    }

    CustomPossible &CustomPossible::operator=(const uint8_t &other)
    {
        dbg_write(other);
        possible_t::operator=(other);
        return *this;
    }

    CustomPossible &CustomPossible::operator=(const possible_t &other)
    {
        dbg_write(other);
        possible_t::operator=(other);
        return *this;
    }

}