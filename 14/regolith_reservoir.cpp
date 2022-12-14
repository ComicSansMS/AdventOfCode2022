#include <regolith_reservoir.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/split_when.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <string>
#include <tuple>

namespace {
inline constinit const Point sand_spawn{ .x = 500, .y = 0 };
}

Point parsePoint(auto rng_point)
{
    auto r = rng_point | ranges::views::split(',');
    auto it = ranges::begin(r);
    assert(it != ranges::end(r));
    int const x = std::stoi(*it | ranges::to<std::string>);
    ++it;
    assert(it != ranges::end(r));
    int const y = std::stoi(*it | ranges::to<std::string>);
    ++it;
    assert(it == ranges::end(r));
    return Point{ x, y };
}

Line parseLine(auto rng_line)
{
    Line ret;
    Point prev = {};
    bool first = true;
    for (auto point_str : rng_line |
        ranges::views::split_when([](char c) { return (c == ' ') || (c == '-') || (c == '>');  }))
    {
        Point const p = parsePoint(point_str);
        if (first) {
            first = false;
        } else {
            ret.segments.push_back(LineSegment{ .start = prev, .end = p });
        }
        prev = p;
    }
    return ret;
}

std::vector<Line> parseInput(std::string_view input)
{
    std::vector<Line> ret;
    for (auto line : input | ranges::views::split('\n')) {
        ret.emplace_back(parseLine(line));
    }
    return ret;
}

std::tuple<Point, Point> determineBounds(std::vector<Line> const& lines)
{
    Point bmin = sand_spawn;
    Point bmax = sand_spawn;
    auto checkPoint = [&bmin, &bmax](Point const& p) {
        bmin.x = std::min(bmin.x, p.x);
        bmin.y = std::min(bmin.y, p.y);
        bmax.x = std::max(bmax.x, p.x);
        bmax.y = std::max(bmax.y, p.y);
    };
    for (auto const& ls : lines) {
        for (auto const& l : ls.segments) {
            checkPoint(l.start);
            checkPoint(l.end);
        }
    }
    return std::make_tuple(bmin, bmax);
}

void drawVerticalLine(FieldSpan f, Point start, Point stop)
{
    assert(start.x == stop.x);
    int const ix = start.x;
    auto const [start_y, stop_y] = std::minmax(start.y, stop.y);
    for (int iy = start_y; iy <= stop_y; ++iy) {
        f(ix, iy) = Cell::Rock;
    }
}

void drawHorizontalLine(FieldSpan f, Point start, Point stop)
{
    assert(start.y == stop.y);
    int const iy = start.y;
    auto const [start_x, stop_x] = std::minmax(start.x, stop.x);
    for (int ix = start_x; ix <= stop_x; ++ix) {
        f(ix, iy) = Cell::Rock;
    }
}

void drawLine(FieldSpan f, LineSegment const& line_segment)
{
    auto const [start, stop] = line_segment;
    if (start.x == stop.x) {
        drawVerticalLine(f, start, stop);
    } else {
        drawHorizontalLine(f, start, stop);
    }
}

void drawLine(FieldSpan f, Line const& line)
{
    for (auto const ls : line.segments) {
        drawLine(f, ls);
    }
}

Field::Field(Point const& bmin, Point const& bmax)
    :bound_min(bmin), bound_max(bmax), width((bmax.x - bmin.x) + 1), height((bmax.y - bmin.y) + 1),
     data(width* height, Cell::Empty),
     field(data.data(), FieldSpanLayout::mapping<std::experimental::dextents<int, 2>>{bmin, width, height})
{
}

Field buildField(std::vector<Line> const& lines)
{
    auto const [bmin, bmax] = determineBounds(lines);
    Field ret(bmin, bmax);
    for (auto const l : lines) {
        drawLine(ret.field, l);
    }
    return ret;
}

bool dropSand(Field& f)
{
    Point sand = sand_spawn;
    auto isInBounds = [&f](Point const& p) {
        return (p.x >= f.bound_min.x) && (p.x <= f.bound_max.x) && (p.y >= f.bound_min.y) && (p.y <= f.bound_max.y);
    };
    for (;;) {
        // down
        {
            Point const p{ sand.x, sand.y + 1 };
            if (!isInBounds(p)) { return false; }
            if (f.field(p.x, p.y) == Cell::Empty) {
                sand = p;
                continue;
            }
        }
        // down left
        {
            Point const p{ sand.x - 1, sand.y + 1 };
            if (!isInBounds(p)) { return false; }
            if (f.field(p.x, p.y) == Cell::Empty) {
                sand = p;
                continue;
            }
        }
        // down right
        {
            Point const p{ sand.x + 1, sand.y + 1 };
            if (!isInBounds(p)) { return false; }
            if (f.field(p.x, p.y) == Cell::Empty) {
                sand = p;
                continue;
            }
        }
        // sand comes to rest at position
        f.field(sand.x, sand.y) = Cell::Sand;
        if (sand == sand_spawn) { return false; }
        break;
    }
    return true;
}

int answer1(Field& f)
{
    int count = 0;
    while (dropSand(f)) { ++count; }
    return count;
}

int answer2(std::vector<Line> const& lines)
{
    auto const [bmin, bmax] = determineBounds(lines);
    auto ls = lines;
    Line bottom;
    bottom.segments.push_back(LineSegment{ .start = { bmin.x - 150, bmax.y + 2 },
                                           .end =   { bmax.x + 150, bmax.y + 2 } });
    ls.push_back(bottom);
    auto f = buildField(ls);
    return answer1(f) + 1;
}
