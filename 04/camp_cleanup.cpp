#include <camp_cleanup.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/count_if.hpp>

#include <cassert>
#include <regex>
#include <string>
#include <tuple>

std::vector<Assignment> parseInput(std::string_view input)
{
    std::regex rx_line(R"((\d+)-(\d+),(\d+)-(\d+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<Assignment> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> Assignment {
            return Assignment {
                .first = { .first = std::stoi(match[1]), .last = std::stoi(match[2]) },
                .second = { .first = std::stoi(match[3]), .last = std::stoi(match[4]) }
            };
        });
    return ret;
}

bool fullyContainedRange(Assignment const& a)
{
    auto const fullyContains = [](Range r1, Range r2) -> bool {
        return (r1.first <= r2.first) && (r1.last >= r2.last);
    };
    return fullyContains(a.first, a.second) || fullyContains(a.second, a.first);
}

int64_t answer1(std::vector<Assignment> const& v)
{
    return ranges::count_if(v, fullyContainedRange);
}

bool doesOverlap(Assignment const& a)
{
    auto const [left, right] = (a.first.first < a.second.first) ? 
        std::make_tuple(a.first, a.second) :
        std::make_tuple(a.second, a.first);
    return (left.last >= right.first);
}

int64_t answer2(std::vector<Assignment> const& v)
{
    return ranges::count_if(v, doesOverlap);
}
