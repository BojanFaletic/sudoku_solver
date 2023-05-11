#pragma once

#include "sud/types.hpp"
#include "sud/sudoku.hpp"
#include <bitset>
#include <iostream>

namespace sud::sol
{
    using possible_t = std::bitset<SUDOKU_POSSIBLE_NUMBERS>;

    class CustomPossible : public possible_t
    {
    private:
        const Point pt;
        void dbg_write(const possible_t &other) const;

    public:
        using possible_t::possible_t;
        CustomPossible(const Point &pt, const possible_t &possible);

        CustomPossible &operator=(const possible_t &other);
        CustomPossible &operator=(const uint8_t &other);
    };

    class Possible
    {
    private:
        std::array<std::array<CustomPossible, SUDOKU_SIZE>, SUDOKU_SIZE>
            possible_board;

    public:
        CustomPossible operator[](const Point &p);
        CustomPossible operator[](const Point &p) const;
    };

} // namespace sud::sol


// Format function for std::bitset
template <>
struct fmt::formatter<sud::sol::possible_t> : fmt::formatter<std::string_view>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx)
    {
        return fmt::formatter<std::string_view>::parse(ctx);
    }

    // Format function for std::bitset
    template <typename FormatContext>
    auto format(const sud::sol::possible_t &bits, FormatContext &ctx)
    {
        return fmt::formatter<std::string_view>::format(bits.to_string(), ctx);
    }
};

namespace fmt
{
    template <>
    struct formatter<sud::sol::CustomPossible>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext &ctx)
        {
            return ctx.begin();
        }

        // Format function for CustomPossible
        template <typename FormatContext>
        auto format(const sud::sol::CustomPossible &possible, FormatContext &ctx)
        {
            // Format the desired output
            return format_to(ctx.out(), "{}", static_cast<sud::sol::possible_t>(possible));
        }
    };
} // namespace fmt
