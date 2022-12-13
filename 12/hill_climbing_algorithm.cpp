#include <hill_climbing_algorithm.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <cassert>
#include <deque>
#include <unordered_set>

Map parseInput(std::string_view input)
{
    Map ret{};
    int iy = 0;
    int width = 0;
    for (auto line : input | ranges::views::split('\n')) {
        int ix = 0;
        for (char c : line) {
            if (c == 'S') {
                assert(ret.start == Point{});
                ret.data.push_back(0);
                ret.start = Point{ .x = ix, .y = iy };
            } else if (c == 'E') {
                assert(ret.end == Point{});
                ret.data.push_back(25);
                ret.end = Point{ .x = ix, .y = iy };
            } else {
                assert((c >= 'a') && (c <= 'z'));
                ret.data.push_back(c - 'a');
            }
            ++ix;
        }
        if (width == 0) { width = ix; }
        assert(ix == width);
        ++iy;
    }
    ret.map = MapSpan{ ret.data.data(), width, iy };
    return ret;
}

AllPaths allPaths(Map const& m, std::vector<Point> const& start_points)
{
    struct StackElement {
        Point p;
        int distance;
    };
    std::deque<StackElement> stack;
    std::unordered_set<Point, HashPoint> visited;
    auto const addPoint = [&stack, &visited](Point const& p, int distance) {
        visited.insert(p);
        stack.emplace_back(StackElement{ p, distance });
    };
    for (auto const& p : start_points) { addPoint(p, 0); }

    auto const is_in_bounds = [width = m.map.extent(0), height = m.map.extent(1)](Point const& p) {
        return (p.x >= 0) && (p.x < width) && (p.y >= 0) && (p.y < height);
    };
    auto const reachable = [map = m.map](Point const& from, Point const& to) -> bool {
        int const height_from = map(from.x, from.y);
        int const height_to = map(to.x, to.y);
        return height_to <= (height_from + 1);
    };
    auto const try_point = [is_in_bounds, reachable, addPoint, &visited]
        (Point const& from, Point const& p, int distance) {
            if (is_in_bounds(p) && !visited.contains(p) && reachable(from, p)) {
                addPoint(p, distance);
            }
        };

    AllPaths ret;
    ret.data.resize(m.data.size(), -1);
    ret.distances = MapSpan{ ret.data.data(), m.map.extents() };
    while (!stack.empty()) {
        auto const [p, p_distance] = stack.front();
        stack.pop_front();
        ret.distances(p.x, p.y) = p_distance;
        int const distance = p_distance + 1;
        // up
        try_point(p, Point{ .x = p.x, .y = p.y - 1 }, distance);
        // down
        try_point(p, Point{ .x = p.x, .y = p.y + 1 }, distance);
        // left
        try_point(p, Point{ .x = p.x - 1, .y = p.y }, distance);
        // right
        try_point(p, Point{ .x = p.x + 1, .y = p.y }, distance);
    }
    return ret;
}

AllPaths allPathsFromStart(Map const& m)
{
    return allPaths(m, std::vector<Point>{ m.start });
}

int answer1(Map const& m)
{
    return allPathsFromStart(m).distances(m.end.x, m.end.y);
}

AllPaths allPathsFromA(Map const& m)
{
    std::vector<Point> const start_points =
        ranges::views::cartesian_product(ranges::views::iota(0, m.map.extent(1)),
                                         ranges::views::iota(0, m.map.extent(0)))
        | ranges::views::transform([](auto yx) -> Point { auto const [y, x] = yx; return Point{ x, y }; })
        | ranges::views::filter([map = m.map](Point p) -> bool { return map(p.x, p.y) == 0; })
        | ranges::to<std::vector>;
    return allPaths(m, start_points);
}

int answer2(Map const& m)
{
    return allPathsFromA(m).distances(m.end.x, m.end.y);
}
