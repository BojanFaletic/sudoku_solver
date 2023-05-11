#pragma once

#include "sud/types.hpp"
#include "sud/sudoku.hpp"
#include <bitset>
#include <iostream>

namespace sud::sol
{
    using possible_t = std::bitset<SUDOKU_POSSIBLE_NUMBERS>;

    class CustomPossible
    {
    private:
        const Point &pt;
        possible_t &possible;

        void dbg_write(const possible_t &other) const;

    public:
        CustomPossible() = delete;
        CustomPossible(const Point &pt, possible_t &possible);
        CustomPossible(const Point &pt, const possible_t &possible);

        possible_t &get() const;
        void reset();
        bool test(const uint8_t &index) const;
        uint8_t count() const;

        possible_t::reference operator[](const uint8_t &index);

        CustomPossible &operator=(const possible_t &other);
        CustomPossible &operator=(const uint8_t &other);

        CustomPossible &operator&=(const bool &other);
    };

    class Possible
    {
    private:
        std::array<std::array<possible_t, SUDOKU_SIZE>, SUDOKU_SIZE>
            possible_board;

    public:
        Possible();
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
            return format_to(ctx.out(), "{}", static_cast<sud::sol::possible_t>(possible.get()));
        }
    };
} // namespace fmt
