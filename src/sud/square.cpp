#include "sud/square.hpp"
#include "sud/types.hpp"
#include <stdexcept>

namespace sud{
    Square::Square(const std::uint8_t value) : value(value)
    {
        if (value > SUDOKU_SIZE)
        {
            throw std::invalid_argument("Square value cannot be greater than SUDOKU_SIZE");
        }
    }

    std::uint8_t Square::get_value() const
    {
        return value;
    }

    bool Square::operator==(const Square &other) const
    {
        return value == other.value;
    }

    bool Square::operator!=(const Square &other) const
    {
        return value != other.value;
    }

    std::ostream &operator<<(std::ostream &os, const Square &square)
    {
        os << static_cast<int>(square.value);
        return os;
    }

} // namespace sud