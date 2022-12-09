#ifndef ADVENT_OF_CODE_09_ROPE_BRIDGE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_09_ROPE_BRIDGE_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <vector>

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct Instruction {
    Direction direction;
    int amount;

    friend bool operator==(Instruction const&, Instruction const&) = default;
};

std::vector<Instruction> parseInput(std::string_view input);

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct Point {
    int x;                                                  // LCOV_EXCL_LINE
    int y;                                                  // LCOV_EXCL_LINE

    friend bool operator==(Point, Point) = default;         // LCOV_EXCL_LINE
};

template<>
struct fmt::formatter<Point>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Point const& p, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

inline std::ostream& operator<<(std::ostream& os, Point const& p) {
    return os << fmt::format("{}", p);
}

struct HashPoint {
    inline std::size_t operator()(Point const& v) const {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        return seed;
    }
};

template<std::size_t N>
struct Rope {
    static_assert(N > 1, "Rope must be at least two knots in length");
    static_assert(N <= 10, "Rope must be at most ten knots in length");
    std::array<Point, N> knots = {};
    std::unordered_set<Point, HashPoint> visited;

    Point& head() {
        return knots.front();
    }

    Point& tail() {
        return knots.back();
    }
};

inline void movePoint(Point& p, Direction dir)
{
    switch (dir)
    {
    case Direction::Up:
        ++p.y;
        break;
    case Direction::Down:
        --p.y;
        break;
    case Direction::Left:
        --p.x;
        break;
    case Direction::Right:
        ++p.x;
        break;
    default: assert(!"Invalid direction"); break;       // LCOV_EXCL_LINE
    }
}

template<std::size_t N>
void moveHead(Rope<N>& rope, Direction dir)
{
    movePoint(rope.head(), dir);
    for (std::size_t i = 0; i < N - 1; ++i) {
        Point const& head = rope.knots[i];
        Point& tail = rope.knots[i + 1];
        int const dx = head.x - tail.x;
        int const dy = head.y - tail.y;
        int const distance = std::max(std::abs(dx), std::abs(dy));
        if (distance > 1) {
            if (dx == 0) {
                tail.y += (dy > 0) ? 1 : -1;
            } else if (dy == 0) {
                tail.x += (dx > 0) ? 1 : -1;
            } else {
                // move diagonally
                tail.x += (dx > 0) ? 1 : -1;
                tail.y += (dy > 0) ? 1 : -1;
            }
            
        }
    }    
    rope.visited.insert(rope.tail());
}

template<std::size_t N>
void moveHead(Rope<N>& rope, Instruction const& move_instruction)
{
    for (int i = 0; i < move_instruction.amount; ++i) {
        moveHead(rope, move_instruction.direction);
    }
}

std::int64_t answer1(std::vector<Instruction> const& instructions);

std::int64_t answer2(std::vector<Instruction> const& instructions);

template<std::size_t N>
struct Field {
    int min_x;
    int max_x;
    int min_y;
    int max_y;
    Rope<N> r;
};

template<std::size_t N>
struct fmt::formatter<Field<N>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Field<N> const& f, FormatContext& ctx) {
        assert((f.min_x <= f.max_x) && (f.min_y <= f.max_y));
        for (int iy = f.max_y; iy >= f.min_y; --iy) {
            for (int ix = f.min_x; ix <= f.max_x; ++ix) {
                char c = '.';
                if ((ix == 0) && (iy == 0)) {
                    c = 's';
                }
                for (int i = N - 1; i >= 0; --i) {
                    Point const& p = f.r.knots[i];
                    if ((ix == p.x) && (iy == p.y)) {
                        c = (i == 0) ? 'H' : static_cast<char>('0' + i);
                    }
                }
                fmt::format_to(ctx.out(), "{}", c);
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

#endif
