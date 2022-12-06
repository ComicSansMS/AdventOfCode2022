#include <tuning_trouble.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/view/sliding.hpp>

std::string parseInput(std::string_view input)
{
    auto it_end = ranges::find(input, '\n');
    return std::string{ ranges::begin(input), it_end };
}

std::string::const_iterator findMarker(std::string const& str, int window_size)
{
    auto const it = ranges::find_if(str | ranges::views::sliding(window_size), [](auto w) {
            bool check[256] = {};
            for (char const c : w) {
                if (check[c]) { return false; } else { check[c] = true; }
            }
            return true;
        });
    return it.base() + window_size;
}

std::size_t answer1(std::string const& str)
{
    return ranges::distance(ranges::begin(str), findMarker(str, 4));
}

std::size_t answer2(std::string const& str)
{
    return ranges::distance(ranges::begin(str), findMarker(str, 14));
}
