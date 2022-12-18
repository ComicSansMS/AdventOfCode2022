#include <boiling_boulders.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <concepts>
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

template<std::invocable<Point const&> Func>
void visitNeighbours(Point const& p, Func&& visitor)
{
    // +x
    visitor(Point{ .x = p.x + 1, .y = p.y, .z = p.z });
    // -x
    visitor(Point{ .x = p.x - 1, .y = p.y, .z = p.z });
    // +y
    visitor(Point{ .x = p.x, .y = p.y + 1, .z = p.z });
    // -y
    visitor(Point{ .x = p.x, .y = p.y - 1, .z = p.z });
    // +z
    visitor(Point{ .x = p.x, .y = p.y, .z = p.z + 1 });
    // -z
    visitor(Point{ .x = p.x, .y = p.y, .z = p.z - 1 });
}

int answer1(std::vector<Point> const& points)
{
    std::unordered_set<Point, HashPoint> pset;
    for (auto const& p : points) {
        pset.insert(p);
    }

    int count = 0;
    for (auto const& p : pset) {
        visitNeighbours(p, [&pset, &count](Point const& pn) {
                if (!pset.contains(pn)) { ++count; }
            });
    }
    return count;
}

int answer2(std::vector<Point> const& points)
{
    Point pmin{ .x = 999, .y = 999, .z = 999 };
    Point pmax{ .x = 0, .y = 0, .z = 0 };
    std::unordered_set<Point, HashPoint> pset;
    for (auto const& p : points) {
        pmin.x = std::min(pmin.x, p.x);
        pmax.x = std::max(pmax.x, p.x);
        pmin.y = std::min(pmin.y, p.y);
        pmax.y = std::max(pmax.y, p.y);
        pmin.z = std::min(pmin.z, p.z);
        pmax.z = std::max(pmax.z, p.z);
        pset.insert(p);
    }
    --pmin.x;
    --pmin.y;
    --pmin.z;
    ++pmax.x;
    ++pmax.y;
    ++pmax.z;

    // flood fill bounding volume
    std::unordered_set<Point, HashPoint> water;
    std::vector<Point> stack;
    stack.push_back(Point{ .x = pmin.x, .y = pmin.y, .z = pmin.z });
    auto isInBounds = [pmin, pmax](Point const& p) -> bool {
        return (p.x >= pmin.x) && (p.x <= pmax.x) &&
               (p.y >= pmin.y) && (p.y <= pmax.y) &&
               (p.z >= pmin.z) && (p.z <= pmax.z);
    };
    auto checkAndPushToStack = [isInBounds, &pset, &water, &stack](Point const& p) {
        if (isInBounds(p) && !pset.contains(p) && !water.contains(p) &&
            (std::find(stack.begin(), stack.end(), p) == stack.end()))
        {
            stack.push_back(p);
        }
    };
    while (!stack.empty()) {
        Point const p = stack.back();
        stack.pop_back();
        water.insert(p);
        visitNeighbours(p, checkAndPushToStack);
    }

    int count = 0;
    auto checkAndCount = [&pset, &water, &count](Point const& p) {
        if ((!pset.contains(p)) && (water.contains(p))) {
            ++count;
        }
    };
    for (auto const& p : pset) {
        visitNeighbours(p, checkAndCount);
    }
    return count;
}
