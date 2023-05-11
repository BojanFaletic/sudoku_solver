#include "sud/sol/possible.hpp"

namespace sud::sol
{
    /* ------------------------------------------------ */
    /* ------------------ Possible ------------------- */
    /* ------------------------------------------------ */

    Possible::Possible()
    {
        for (uint8_t row = 0; row < SUDOKU_SIZE; row++)
        {
            for (uint8_t col = 0; col < SUDOKU_SIZE; col++)
            {
                possible_board[row][col] = 0;
            }
        }
    }

    CustomPossible Possible::operator[](const Point &p)
    {
        CustomPossible possible = {p, std::ref(possible_board[p.row][p.col])};
        return possible;
    }

    CustomPossible Possible::operator[](const Point &p) const
    {
        const CustomPossible possible = {p, possible_board[p.row][p.col]};
        return possible;
    }

    /* ------------------------------------------------ */
    /* ---------------- CustomPossible ---------------- */
    /* ------------------------------------------------ */

    CustomPossible::CustomPossible(const Point &pt, possible_t &possible) : possible(possible), pt(pt)
    {
    }

    CustomPossible::CustomPossible(const Point &pt, const possible_t &possible) : possible(const_cast<possible_t &>(possible)), pt(pt)
    {
    }

    void CustomPossible::dbg_write(const possible_t &other) const
    {
        std::set<uint8_t> prev;
        std::set<uint8_t> other_set;

        for (uint8_t i = 0; i < SUDOKU_POSSIBLE_NUMBERS; i++)
        {
            if (possible.test(i))
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

    possible_t &CustomPossible::get() const
    {
        return possible;
    }

    void CustomPossible::reset()
    {
        possible.reset();
    }

    bool CustomPossible::test(const uint8_t &index) const
    {
        return possible.test(index);
    }

    possible_t::reference CustomPossible::operator[](const uint8_t &index)
    {
        return possible[index];
    }

    uint8_t CustomPossible::count() const
    {
        return possible.count();
    }

    CustomPossible &CustomPossible::operator=(const uint8_t &other)
    {
        dbg_write(other);
        possible = other;
        return *this;
    }

    CustomPossible &CustomPossible::operator=(const possible_t &other)
    {
        dbg_write(other);
        possible = other;
        return *this;
    }

    CustomPossible &CustomPossible::operator&=(const bool &other)
    {
        dbg_write(other);
        const possible_t temp = (other) ? std::bitset<SUDOKU_POSSIBLE_NUMBERS>(0b111111111) : std::bitset<SUDOKU_POSSIBLE_NUMBERS>(0b000000000);
        possible &= temp;
        return *this;
    }
}