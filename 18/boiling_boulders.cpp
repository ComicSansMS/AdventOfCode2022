#include <boiling_boulders.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <regex>


std::vector<Point> parseInput(std::string_view input)
{
    std::regex rx_line(R"((\d+),(\d+),(\d+))");
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    return ranges::make_subrange(it_begin, it_end) | ranges::views::transform(
        [](std::match_results<std::string_view::iterator> const& match) -> Point {
            return Point{
                    .x = std::stoi(match[1]),
                    .y = std::stoi(match[2]),
                    .z = std::stoi(match[3]),
        };
        }) | ranges::to<std::vector>;
}

int answer1(std::vector<Point> const& points)
{
    std::unordered_set<Point, HashPoint> pset;
    for (auto const& p : points) {
        pset.insert(p);
    }

    int count = 0;
    for (auto const& p : pset) {
        // +x
        if (!pset.contains(Point{ .x = p.x + 1, .y = p.y, .z = p.z })) { ++count; }
        // -x
        if (!pset.contains(Point{ .x = p.x - 1, .y = p.y, .z = p.z })) { ++count; }
        // +y
        if (!pset.contains(Point{ .x = p.x, .y = p.y + 1, .z = p.z })) { ++count; }
        // -y
        if (!pset.contains(Point{ .x = p.x, .y = p.y - 1, .z = p.z })) { ++count; }
        // +z
        if (!pset.contains(Point{ .x = p.x, .y = p.y, .z = p.z + 1 })) { ++count; }
        // -z
        if (!pset.contains(Point{ .x = p.x, .y = p.y, .z = p.z - 1 })) { ++count; }
    }
    return count;
}
