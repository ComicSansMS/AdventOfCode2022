#ifndef ADVENT_OF_CODE_25_FULL_OF_HOT_AIR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_25_FULL_OF_HOT_AIR_HPP_INCLUDE_GUARD

#include <range/v3/view/reverse.hpp>

#include <fmt/format.h>

#include <cassert>
#include <cstdint>
#include <string_view>
#include <vector>

enum class SnafuDigit {
    Zero = 0,
    One,
    Two,
    MinusTwo,
    MinusOne,
};

template<>
struct fmt::formatter<SnafuDigit>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(SnafuDigit d, FormatContext& ctx) {
        char c = '?';
        if (d == SnafuDigit::Zero) {
            c = '0';
        } else if (d == SnafuDigit::One) {
            c = '1';
        } else if (d == SnafuDigit::Two) {
            c = '2';
        } else if (d == SnafuDigit::MinusOne) {
            c = '-';
        } else {
            assert(d == SnafuDigit::MinusTwo);          // LCOV_EXCL_LINE
            c = '=';

        }
        return fmt::format_to(ctx.out(), "{}", c);
    }
};

struct SnafuNumber {
    std::vector<SnafuDigit> digits;

    friend bool operator==(SnafuNumber const&, SnafuNumber const&) = default;
};

template<>
struct fmt::formatter<SnafuNumber>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(SnafuNumber const& s, FormatContext& ctx) {
        for (SnafuDigit const& d : s.digits | ranges::views::reverse) {
            fmt::format_to(ctx.out(), "{}", d);
        }
        return ctx.out();
    }
};

SnafuNumber parseNumber(std::string_view str);

std::vector<SnafuNumber> parseInput(std::string_view input);

std::int64_t snafuToDecimal(SnafuNumber const& s);

SnafuNumber decimalToSnafu(std::int64_t n);

SnafuNumber answer(std::vector<SnafuNumber> const& numbers);

#endif
