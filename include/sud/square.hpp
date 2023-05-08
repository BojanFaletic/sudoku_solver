#pragma once

#include <cstdint>
#include <ostream>

namespace sud
{
    class Square
    {
    private:
        const std::uint8_t value;

    public:
        Square(const std::uint8_t value);

        // getters
        std::uint8_t get_value() const;

        // operators
        bool operator==(const Square &other) const;
        bool operator!=(const Square &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Square &square);
    };

} // namespace sud
