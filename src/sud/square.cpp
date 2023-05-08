#include "sud/square.hpp"
#include "sud/types.hpp"
#include <stdexcept>

namespace sud
{
    Square::Square() : value(0) {}

    Square::Square(const std::uint8_t value) : value(value)
    {
        if (value > SUDOKU_SIZE)
        {
            throw std::invalid_argument("Square value cannot be greater than SUDOKU_SIZE");
        }
    }

    Square Square::get() const
    {
        return *this;
    }

    std::uint8_t Square::to_value() const
    {
        return value;
    }

    Square Square::set(const Square &other)
    {
        value = other.value;
        return *this;
    }

    bool Square::operator==(const Square &other) const
    {
        return value == other.value;
    }

    bool Square::operator!=(const Square &other) const
    {
        return value != other.value;
    }

    Square::operator bool() const
    {
        return value != 0;
    }

    std::ostream &operator<<(std::ostream &os, const Square &square)
    {
        os << static_cast<int>(square.value);
        return os;
    }

} // namespace sud