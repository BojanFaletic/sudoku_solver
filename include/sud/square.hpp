#pragma once

#include <cstdint>
#include <ostream>
#include <fmt/format.h>

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

namespace fmt
{
    template <>
    struct formatter<sud::Square>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

        template <typename FormatContext>
        auto format(const sud::Square &square, FormatContext &ctx) const
        {
            return fmt::format_to(ctx.out(), "{}",
                                  static_cast<int>(square.to_value()));
        }
    };
}
