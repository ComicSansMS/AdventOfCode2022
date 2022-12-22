#include <monkey_map.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>

Cell parseLine(char c)
{
    if (c == ' ') {
        return Cell::Void;
    } else if (c == '.') {
        return Cell::Empty;
    } else {
        assert(c == '#');
        return Cell::Wall;
    }
}

Path parsePath(auto r)
{
    Path ret;
    std::string n;
    for (auto it = ranges::begin(r); it != ranges::end(r); ++it) {
        char const c = *it;
        if (std::isdigit(c)) {
            n.push_back(c);
        } else {
            assert((c == 'L') || (c == 'R'));
            assert(!n.empty());
            ret.push_back(Forward{ .amount = std::stoi(n) });
            n.clear();
            ret.push_back((c == 'L') ? Turn::Left : Turn::Right);
        }
    }
    assert(!n.empty());
    ret.push_back(Forward{ .amount = std::stoi(n) });
    return ret;
}

Input parseInput(std::string_view input)
{
    std::vector<std::vector<Cell>> lines;
    Path p;
    int max_width = 0;
    enum class ParseState {
        ExpectLine,
        ExpectPath,
        ExpectEnd
    } state = ParseState::ExpectLine;
    for (auto line : input | ranges::views::split('\n')) {
        if (state == ParseState::ExpectLine) {
            if (line.empty()) {
                state = ParseState::ExpectPath;
                continue;
            }
            lines.push_back(line | ranges::views::transform(parseLine) | ranges::to<std::vector>);
            max_width = std::max(max_width, static_cast<int>(lines.back().size()));
        } else {
            assert(state == ParseState::ExpectPath);
            p = parsePath(line);
            state = ParseState::ExpectEnd;
        }
    }
    assert(state == ParseState::ExpectEnd);

    Map m;
    // we pad the map with an additional row/column of Void cells all around
    m.width = max_width + 2;
    m.height = static_cast<int>(lines.size()) + 2;
    m.data.reserve(m.width * m.height);
    m.data.assign(m.width, Cell::Void);
    for (auto const& l : lines) {
        m.data.push_back(Cell::Void);
        m.data.insert(m.data.end(), l.begin(), l.end());
        for (auto i = l.size() + 1; i < m.width; ++i) { m.data.push_back(Cell::Void); }
    }
    m.data.insert(m.data.end(), m.width, Cell::Void);
    assert(static_cast<int>(m.data.size()) == m.width * m.height);

    m.row_width.resize(m.height, Range{ .first = -1, .last = -1 });
    m.col_height.resize(m.width, Range{ .first = -1, .last = -1 });
    auto const mds = mapSpan(m);
    for (int iy = 0; iy < m.height; ++iy) {
        Range& rrow = m.row_width[iy];
        for (int ix = 0; ix < m.width; ++ix) {
            Range& rcol = m.col_height[ix];
            if (mds(ix, iy) != Cell::Void) {
                if (rrow.first == -1) { rrow.first = ix; }
                if (rcol.first == -1) { rcol.first = iy; }
            } else {
                if ((rrow.first != -1) && (rrow.last == -1)) { rrow.last = ix - 1; }
                if ((rcol.first != -1) && (rcol.last == -1)) { rcol.last = iy - 1; }
            }
        }
    }

    return Input{ .map = std::move(m), .path = std::move(p)};
}

Position determineStartingPosition(Map const& m)
{
    // assume the top left spot is not a wall
    assert(mapSpan(m)(m.row_width[1].first, 1) == Cell::Empty);
    return Position{ .x = m.row_width[1].first, .y = 1, .dir = Direction::Right };
}

Direction turnLeft(Direction d)
{
    return static_cast<Direction>((static_cast<int>(d) + 3) % 4);
}

Direction turnRight(Direction d)
{
    return static_cast<Direction>((static_cast<int>(d) + 1) % 4);

}

int getDxForDirection(Direction d)
{
    if (d == Direction::Left) {
        return -1;
    } else if (d == Direction::Right) {
        return 1;
    }
    return 0;
}

int getDyForDirection(Direction d)
{
    if (d == Direction::Up) {
        return -1;
    } else if (d == Direction::Down) {
        return 1;
    }
    return 0;
}

Position wrapAroundFlat(Map const& map, Position p)
{
    if (p.dir == Direction::Right) {
        p.x = map.row_width[p.y].first;
    } else if (p.dir == Direction::Left) {
        p.x = map.row_width[p.y].last;
    } else if (p.dir == Direction::Up) {
        p.y = map.col_height[p.x].last;
    } else {
        assert(p.dir == Direction::Down);
        p.y = map.col_height[p.x].first;
    }
    return p;
}

template<std::regular_invocable<Map const&, Position> WrapAroundFunc>
struct WalkVariant {
    Position pos;
    Map const& map;
    WrapAroundFunc* wrapAroundFunc;
    WalkVariant(Position const& s, Map const& m, WrapAroundFunc&& func)
        :pos(s), map(m), wrapAroundFunc(func)
    {}
    Position operator()(Turn t) {
        pos.dir = (t == Turn::Left) ? turnLeft(pos.dir) : turnRight(pos.dir);
        return pos;
    }
    Position operator()(Forward f) {
        auto mds = mapSpan(map);
        for (int i = 0; i < f.amount; ++i) {
            Position new_pos = pos;
            new_pos.x += getDxForDirection(pos.dir);
            new_pos.y += getDyForDirection(pos.dir);
            Cell new_cell = mds(new_pos.x, new_pos.y);
            if (new_cell == Cell::Void) {
                // wrap around
                new_pos = wrapAroundFunc(map, new_pos);
                new_cell = mds(new_pos.x, new_pos.y);
            }
            assert(new_cell != Cell::Void);
            if (new_cell == Cell::Wall) { break; }
            pos = new_pos;
        }
        return pos;
    }
};

Position walk(Position const& start, Map const& m, Step const& s)
{
    return std::visit(WalkVariant{ start, m, wrapAroundFlat }, s);
}

int scorePosition(Position const& pos)
{
    return (pos.y * 1000) + (pos.x * 4) + static_cast<int>(pos.dir);
}

int answer1(Input const& in)
{
    auto pos = determineStartingPosition(in.map);
    for (auto const& s : in.path) {
        pos = walk(pos, in.map, s);
    }
    return scorePosition(pos);
}

// LCOV_EXCL_START
Position wrapAroundCube(Map const&, Position p)
{
    // Assume the following cube layout:
    // 0     5  10 15
    //       +--+--+
    //       | 2| 1|
    // 5     +--+--+
    //       | 3|
    // 10 +--+--+
    //    | 5| 4|
    // 15 +--+--+
    //    | 6|
    // 20 +--+
    // 
    Range const f1x = Range{ .first = 101, .last = 150 };
    Range const f1y = Range{ .first = 1, .last = 50 };
    Range const f2x = Range{ .first = 51, .last = 100 };
    Range const f2y = Range{ .first = 1, .last = 50 };
    Range const f3x = Range{ .first = 51, .last = 100 };
    Range const f3y = Range{ .first = 51, .last = 100 };
    Range const f4x = Range{ .first = 51, .last = 100 };
    Range const f4y = Range{ .first = 101, .last = 150 };
    Range const f5x = Range{ .first = 1, .last = 50 };
    Range const f5y = Range{ .first = 101, .last = 150 };
    Range const f6x = Range{ .first = 1, .last = 50 };
    Range const f6y = Range{ .first = 151, .last = 200 };
    auto checkBounds = [](Position const& p, Range const& rx, Range const& ry) -> bool {
        return (p.x >= rx.first) && (p.x <= rx.last) && (p.y >= ry.first) && (p.y <= ry.last);
    };
    auto isInBounds = [=](Position const& p, int f) -> bool {
        if (f == 1) {
            return checkBounds(p, f1x, f1y);
        } else if (f == 2) {
            return checkBounds(p, f2x, f2y);
        } else if (f == 3) {
            return checkBounds(p, f3x, f3y);
        } else if (f == 4) {
            return checkBounds(p, f4x, f4y);
        } else if (f == 5) {
            return checkBounds(p, f5x, f5y);
        } else {
            assert(f == 6);
            return checkBounds(p, f6x, f6y);
        }
    };
    Position pn = p;
    if (p.dir == Direction::Right) {
        --p.x;
        if ((p.y >= 1) && (p.y <= 50)) {
            // 1 -> 4
            pn.x = f4x.last;
            pn.y = (49 - (p.y - f1y.first)) + f4y.first;
            pn.dir = Direction::Left;
            assert(isInBounds(pn, 4));
        } else if ((p.y >= 51) && (p.y <= 100)) {
            // 3 -> 1
            pn.x = (p.y - f3y.first) + f1x.first;
            pn.y = f1y.last;
            pn.dir = Direction::Up;
            assert(isInBounds(pn, 1));
        } else if ((p.y >= 101) && (p.y <= 150)) {
            // 4 -> 1
            pn.x = f1x.last;
            pn.y = (49 - (p.y - f4y.first)) + f1y.first;
            pn.dir = Direction::Left;
            assert(isInBounds(pn, 1));
        } else {
            assert((p.y >= 151) && (p.y <= 200));
            // 6 -> 4
            pn.x = (p.y - f6y.first) + f4x.first;
            pn.y = f4y.last;
            pn.dir = Direction::Up;
            assert(isInBounds(pn, 4));
        }
    } else if (p.dir == Direction::Left) {
        ++p.x;
        if ((p.y >= 1) && (p.y <= 50)) {
            // 2 -> 5
            pn.x = f5x.first;
            pn.y = (49 - (p.y - f2y.first)) + f5y.first;
            pn.dir = Direction::Right;
            assert(isInBounds(pn, 5));
        } else if ((p.y >= 51) && (p.y <= 100)) {
            // 3 -> 5
            pn.x = (p.y - f3y.first) + f5x.first;
            pn.y = f5y.first;
            pn.dir = Direction::Down;
            assert(isInBounds(pn, 5));
        } else if ((p.y >= 101) && (p.y <= 150)) {
            // 5 -> 2
            pn.x = f2x.first;
            pn.y = (49 - (p.y - f5y.first)) + f2y.first;
            pn.dir = Direction::Right;
            assert(isInBounds(pn, 2));
        } else {
            assert((p.y >= 151) && (p.y <= 200));
            // 6 -> 2
            pn.x = (p.y - f6y.first) + f2x.first;
            pn.y = f2y.first;
            pn.dir = Direction::Down;
            assert(isInBounds(pn, 2));
        }
    } else if (p.dir == Direction::Up) {
        ++p.y;
        if ((p.x >= 1) && (p.x <= 50)) {
            // 5 -> 3
            pn.x = f3x.first;
            pn.y = (p.x - f5x.first) + f3y.first;
            pn.dir = Direction::Right;
            assert(isInBounds(pn, 3));
        } else if ((p.x >= 51) && (p.x <= 100)) {
            // 2 -> 6
            pn.x = f6x.first;
            pn.y = (p.x - f2x.first) + f6y.first;
            pn.dir = Direction::Right;
            assert(isInBounds(pn, 6));
        } else {
            assert((p.x >= 101) && (p.x <= 150));
            // 1 -> 6
            pn.x = (p.x - f1x.first) + f6x.first;
            pn.y = f6y.last;
            pn.dir = Direction::Up;
            assert(isInBounds(pn, 6));
        }
    } else {
        assert(p.dir == Direction::Down);
        --p.y;
        if ((p.x >= 1) && (p.x <= 50)) {
            // 6 -> 1
            pn.x = (p.x - f6x.first) + f1x.first;
            pn.y = f1y.first;
            pn.dir = Direction::Down;
            assert(isInBounds(pn, 1));
        } else if ((p.x >= 51) && (p.x <= 100)) {
            // 4 -> 6
            pn.x = f6x.last;
            pn.y = (p.x - f4x.first) + f6y.first;
            pn.dir = Direction::Left;
            assert(isInBounds(pn, 6));
        } else {
            assert((p.x >= 101) && (p.x <= 150));
            // 1 -> 3
            pn.x = f3x.last;
            pn.y = (p.x - f1x.first) + f3y.first;
            pn.dir = Direction::Left;
            assert(isInBounds(pn, 3));
        }
    }
    return pn;
}

Position walkCube(Position const& start, Map const& m, Step const& s)
{
    return std::visit(WalkVariant{ start, m, wrapAroundCube }, s);
}

int answer2(Input const& in)
{
    auto pos = determineStartingPosition(in.map);
    for (auto const& s : in.path) {
        pos = walkCube(pos, in.map, s);
    }
    return scorePosition(pos);
}
// LCOV_EXCL_STOP
