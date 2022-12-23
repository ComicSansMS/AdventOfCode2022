#include <unstable_diffusion.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <limits>
#include <unordered_map>

Field parseInput(std::string_view input)
{
    int ix = 0;
    int iy = 0;
    int line_width = -1;
    Field ret{};
    ret.min = Point{ .x = 0, .y = 0 };
    for (char const c : input) {
        if (c == '\n') {
            ++iy;
            if (line_width == -1) {
                line_width = ix;
            }
            assert(ix == line_width);
            ix = 0;
        } else if (c == '#') {
            ret.points.insert(Point{ .x = ix, .y = iy });
            ++ix;
        } else {
            assert(c == '.');
            ++ix;
        }
    }
    ret.max = Point{ .x = line_width, .y = iy };
    return ret;
}

struct ProposedMove {
    Point from;
    Point to;
};

Field move(Field const& f0, int step_count)
{
    Field ret;
    ret.points.reserve(f0.points.size());
    ret.min = Point{ .x = std::numeric_limits<int>::max(), .y = std::numeric_limits<int>::max() };
    ret.max = Point{ .x = std::numeric_limits<int>::min(), .y = std::numeric_limits<int>::min() };
    ret.did_change = false;
    auto insertPoint = [&ret](Point const& p) {
        assert(!ret.points.contains(p));
        ret.points.insert(p);
        ret.max.x = std::max(ret.max.x, p.x + 1);
        ret.max.y = std::max(ret.max.y, p.y + 1);
        ret.min.x = std::min(ret.min.x, p.x);
        ret.min.y = std::min(ret.min.y, p.y);
    };
    auto none_of = [](auto... bs) -> bool { return ((!bs) && ...); };
    std::vector<ProposedMove> moves;
    std::unordered_map<Point, int, HashPoint> dest_count;
    for (auto const& p : f0.points) {
        bool const n = f0.points.contains(Point{ .x = p.x, .y = p.y - 1 });
        bool const s = f0.points.contains(Point{ .x = p.x, .y = p.y + 1 });
        bool const e = f0.points.contains(Point{ .x = p.x + 1, .y = p.y });
        bool const w = f0.points.contains(Point{ .x = p.x - 1, .y = p.y });
        bool const nw = f0.points.contains(Point{ .x = p.x - 1, .y = p.y - 1 });
        bool const ne = f0.points.contains(Point{ .x = p.x + 1, .y = p.y - 1 });
        bool const sw = f0.points.contains(Point{ .x = p.x - 1, .y = p.y + 1 });
        bool const se = f0.points.contains(Point{ .x = p.x + 1, .y = p.y + 1 });
        
        auto tryNorth = [=, &moves, &dest_count](Point const& p) -> bool {
            if (none_of(n, ne, nw)) {
                // move north
                Point const dst{ .x = p.x, .y = p.y - 1 };
                moves.push_back(ProposedMove{ .from = p, .to = dst });
                ++dest_count[dst];
                return true;
            }
            return false;
        };
        auto trySouth = [=, &moves, &dest_count](Point const& p) -> bool {
            if (none_of(s, se, sw)) {
                // move south
                Point const dst{ .x = p.x, .y = p.y + 1 };
                moves.push_back(ProposedMove{ .from = p, .to = dst });
                ++dest_count[dst];
                return true;
            }
            return false;
        };
        auto tryWest = [=, &moves, &dest_count](Point const& p) -> bool {
            if (none_of(w, nw, sw)) {
                // move west
                Point const dst{ .x = p.x - 1, .y = p.y };
                moves.push_back(ProposedMove{ .from = p, .to = dst });
                ++dest_count[dst];
                return true;
            }
            return false;
        };
        auto tryEast = [=, &moves, &dest_count](Point const& p) -> bool {
            if (none_of(e, ne, se)) {
                // move east
                Point const dst{ .x = p.x + 1, .y = p.y };
                moves.push_back(ProposedMove{ .from = p, .to = dst });
                ++dest_count[dst];
                return true;
            }
            return false;
        };

        std::array<std::function<bool(Point const&)>, 4> checkers = { tryNorth, trySouth, tryWest, tryEast };
        if (none_of(n, s, e, w, nw, ne, sw, se) ||
            (! ((checkers[step_count % 4](p)) ||
                      (checkers[(step_count + 1) % 4](p)) ||
                      (checkers[(step_count + 2) % 4](p)) ||
                      (checkers[(step_count + 3) % 4](p)))))
        {
            // stay in place
            insertPoint(p);
        }
    }

    for (auto const& m : moves) {
        int const count = dest_count[m.to];
        assert(count > 0);
        if (count == 1) {
            ret.did_change = true;
            insertPoint(m.to);
        } else {
            insertPoint(m.from);
        }
    }
    assert(ret.points.size() == f0.points.size());
    return ret;
}

int answer1(Field const& field)
{
    Field f = field;
    for (int step = 0; step < 10; ++step) {
        f = move(f, step);
    }
    return ((f.max.y - f.min.y) * (f.max.x - f.min.x)) - static_cast<int>(f.points.size());
}

int answer2(Field const& field)
{
    Field f = field;
    int step = 0;
    for (; ; ++step) {
        f = move(f, step);
        if (!f.did_change) { break; }
    }
    return step + 1;
}

