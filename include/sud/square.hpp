#pragma once

#include <cstdint>
#include <ostream>

namespace sud
{
    class Square
    {
    private:
        std::uint8_t value;

    public:
        Square();
        Square(const std::uint8_t value);

        // getters
        Square get() const;
        Square set(const Square &other);
        std::uint8_t to_value() const;

        // operators
        bool operator==(const Square &other) const;
        bool operator!=(const Square &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Square &square);
        operator bool() const;
    };

} // namespace sud
