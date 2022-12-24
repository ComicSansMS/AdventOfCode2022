#include <blizzard_basin.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <array>
#include <deque>
#include <functional>
#include <limits>
#include <unordered_set>

Cell parseCell(char c)
{
    if (c == '>') {
        return Cell::Right;
    } else if (c == '<') {
        return Cell::Left;
    } else if (c == '^') {
        return Cell::Up;
    } else if (c == 'v') {
        return Cell::Down;
    } else {
        assert(c == '.');
        return Cell::Empty;
    }
}

Field parseInput(std::string_view input)
{
    Field ret{ .map = {}, .width = -1, .height = -1 };
    bool last_line_reached = false;
    for (auto const line : input | ranges::views::split('\n')) {
        std::string l = ranges::to<std::string>(line);
        assert(l.size() > 2);
        if (l.starts_with("#.#")) {
            // first line
            assert(ret.width == -1);
            ret.width = static_cast<int>(l.size()) - 2;
            ret.height = 0;
        } else if (l.ends_with("#.#")) {
            // last line
            assert(static_cast<int>(l.size()) - 2 == ret.width);
            last_line_reached = true;
        } else {
            assert(l.starts_with('#') && l.ends_with('#'));
            assert(static_cast<int>(l.size()) - 2 == ret.width);
            assert(!last_line_reached);
            for (int i = 1; i < static_cast<int>(l.size()) - 1; ++i) {
                char c = l[i];
                ret.map.push_back(parseCell(c));
            }
            ++ret.height;
        }
    }
    return ret;
}

bool containsUp(Cell c)
{
    auto const mask = static_cast<std::underlying_type_t<Cell>>(Cell::Up);
    return (static_cast<std::underlying_type_t<Cell>>(c) & mask) != 0;
}

bool containsDown(Cell c)
{
    auto const mask = static_cast<std::underlying_type_t<Cell>>(Cell::Down);
    return (static_cast<std::underlying_type_t<Cell>>(c) & mask) != 0;
}

bool containsLeft(Cell c)
{
    auto const mask = static_cast<std::underlying_type_t<Cell>>(Cell::Left);
    return (static_cast<std::underlying_type_t<Cell>>(c) & mask) != 0;
}

bool containsRight(Cell c)
{
    auto const mask = static_cast<std::underlying_type_t<Cell>>(Cell::Right);
    return (static_cast<std::underlying_type_t<Cell>>(c) & mask) != 0;
}

void addDirection(Cell& c, Cell dir)
{
    auto const mask = static_cast<std::underlying_type_t<Cell>>(dir);
    auto const c_int = static_cast<std::underlying_type_t<Cell>>(c);
    assert((c_int & mask) == 0);
    c = static_cast<Cell>(c_int | mask);
}

Field advanceTime(Field const& f)
{
    Field ret;
    ret.width = f.width;
    ret.height = f.height;
    ret.map.resize(f.map.size(), Cell::Empty);
    auto const mds_in = fieldSpan(f);
    auto mds_out = fieldSpan(ret);
    for (int iy = 0; iy < mds_in.extent(1); ++iy) {
        for (int ix = 0; ix < mds_in.extent(0); ++ix) {
            Cell const c_in = mds_in(ix, iy);
            if (containsUp(c_in)) {
                int const n_y = (iy != 0) ? (iy - 1) : (ret.height - 1);
                addDirection(mds_out(ix, n_y), Cell::Up);
            }
            if (containsDown(c_in)) {
                int const n_y = (iy != (ret.height - 1)) ? (iy + 1) : 0;
                addDirection(mds_out(ix, n_y), Cell::Down);
            }
            if (containsLeft(c_in)) {
                int const n_x = (ix != 0) ? (ix - 1) : (ret.width - 1);
                addDirection(mds_out(n_x, iy), Cell::Left);
            }
            if (containsRight(c_in)) {
                int const n_x = (ix != (ret.width - 1)) ? (ix + 1) : 0;
                addDirection(mds_out(n_x, iy), Cell::Right);
            }
        }
    }
    return ret;
}

FieldCube buildFieldCube(Field const& field)
{
    FieldCube ret;
    int const depth = (field.width * field.height);
    ret.map.resize(field.width * field.height * depth, Cell::Empty);
    ret.width = field.width;
    ret.height = field.height;
    ret.depth = depth;
    auto mds_out = cubeSpan(ret);
    Field f = field;
    for (int i = 0; i < depth; ++i) {
        auto mds_in = fieldSpan(f);
        for (int iy = 0; iy < mds_in.extent(1); ++iy) {
            for (int ix = 0; ix < mds_in.extent(0); ++ix) {
                mds_out(ix, iy, i) = mds_in(ix, iy);
            }
        }
        f = advanceTime(f);
    }
    assert(f.map == field.map);
    return ret;
}

int solve(FieldCube const& c, Point const& start, Point const& goal)
{
    auto isInBounds = [w = c.width, h = c.height](Point const& p) {
        return ((p.x >= 0) && (p.x < w) && (p.y >= 0) && (p.y < h));
    };
    auto isWinning = [goal](Point const& p) {
        return (p.x == goal.x) && (p.y == goal.y);
    };
    auto csp = cubeSpan(c);
    int start_depth = start.z;
    while (csp(start.x, start.y, start_depth % c.depth) != Cell::Empty) {
        ++start_depth;
    }
    std::deque<Point> posqueue;
    std::unordered_set<Point, HashPoint> visited;
    auto tryPoint = [isInBounds, isWinning, csp, &posqueue, &visited](Point const& p) -> bool {
        if (isWinning(p)) { return true; }
        if (isInBounds(p) && (!visited.contains(p)) && (csp(p.x, p.y, (p.z % csp.extent(2))) == Cell::Empty)) {
            posqueue.push_back(p);
            visited.insert(p);
        }
        return false;
    };

    posqueue.push_back(Point{ .x = start.x, .y = start.y, .z = start_depth });
    int max_depth = -1;
    for (;;) {
        auto p = posqueue.front();
        if (isWinning(p)) {
            max_depth = p.z + 1;
            break;
        }
        posqueue.pop_front();
        int const next_depth = p.z + 1;
        if (tryPoint(Point{ .x = p.x, .y = p.y - 1, .z = next_depth }) ||
            tryPoint(Point{ .x = p.x, .y = p.y + 1, .z = next_depth }) ||
            tryPoint(Point{ .x = p.x - 1, .y = p.y, .z = next_depth }) ||
            tryPoint(Point{ .x = p.x + 1, .y = p.y, .z = next_depth }) ||
            tryPoint(Point{ .x = p.x, .y = p.y, .z = next_depth }))
        {
            max_depth = next_depth + 1;
            break;
        }
        if (posqueue.empty()) {
            ++start_depth;
            posqueue.push_back(Point{ .x = start.x, .y = start.y, .z = start_depth });
        }
    }
    return max_depth;
}

int answer1(FieldCube const& c)
{
    return solve(c, Point{ .x = 0, .y = 0, .z = 1 }, Point{ .x = c.width - 1, .y = c.height - 1, .z = -1 });
}

int answer2(FieldCube const& c)
{
    int const s1 = solve(c, Point{ .x = 0, .y = 0, .z = 1 }, Point{ .x = c.width - 1, .y = c.height - 1, .z = -1 });
    int const s2 = solve(c, Point{ .x = c.width - 1, .y = c.height - 1, .z = s1 + 1 }, Point{ .x = 0, .y = 0, .z = -1 });
    int const s3 = solve(c, Point{ .x = 0, .y = 0, .z = s2 + 1 }, Point{ .x = c.width - 1, .y = c.height - 1, .z = -1 });
    return s3;
}

