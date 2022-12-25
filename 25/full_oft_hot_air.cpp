#include <full_oft_hot_air.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/reverse.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

constexpr std::int64_t pow5(int64_t n) {
    int64_t acc = 1;
    for (int i = 0; i < n; ++i) { acc *= 5; }
    return acc;
}

SnafuDigit parseDigit(char c)
{
    if (c == '0') {
        return SnafuDigit::Zero;
    } else if (c == '1') {
        return SnafuDigit::One;
    } else if (c == '2') {
        return SnafuDigit::Two;
    } else if (c == '-') {
        return SnafuDigit::MinusOne;
    } else {
        assert(c == '=');
        return SnafuDigit::MinusTwo;
    }
}

SnafuNumber parseNumber_impl(auto rng)
{
    std::vector<SnafuDigit> digits = rng
        | ranges::views::transform(parseDigit)
        | ranges::to<std::vector>;
    ranges::reverse(digits);
    return SnafuNumber{ .digits = std::move(digits) };
}

SnafuNumber parseNumber(std::string_view str)
{
    return parseNumber_impl(str);
}

std::vector<SnafuNumber> parseInput(std::string_view input)
{
    return input
        | ranges::views::split('\n')
        | ranges::views::transform([](auto rng) -> SnafuNumber { return parseNumber_impl(rng); })
        | ranges::to<std::vector>;
}

std::int64_t digitToDecimal(SnafuDigit d)
{
    if (d == SnafuDigit::Zero) {
        return 0;
    } else if (d == SnafuDigit::One) {
        return 1;
    } else if (d == SnafuDigit::Two) {
        return 2;
    } else if (d == SnafuDigit::MinusOne) {
        return -1;
    } else {
        assert(d == SnafuDigit::MinusTwo);
        return -2;
    }
}

std::int64_t snafuToDecimal(SnafuNumber const& s)
{
    return ranges::accumulate(s.digits | ranges::views::enumerate, std::int64_t{ 0 }, ranges::plus{},
        [](auto indexdigit) -> std::int64_t {
            auto const [index, digit] = indexdigit;
            return digitToDecimal(digit) * pow5(index);
        });
}

SnafuNumber decimalToSnafu(std::int64_t n)
{
    assert(n >= 0);
    SnafuNumber ret;
    ret.digits.push_back(static_cast<SnafuDigit>(n % 5));
    while (n + 2 >= 5) {
        n = (n + 2) / 5;
        ret.digits.push_back(static_cast<SnafuDigit>(n % 5));
    }
    return ret;
}

SnafuNumber answer(std::vector<SnafuNumber> const& numbers)
{
    return decimalToSnafu(
        ranges::accumulate(numbers | ranges::views::transform(snafuToDecimal),
                           int64_t{ 0 }, ranges::plus{}));
}
