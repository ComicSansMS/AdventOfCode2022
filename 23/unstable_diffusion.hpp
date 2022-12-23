#ifndef ADVENT_OF_CODE_23_UNSTABLE_DIFFUSION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_23_UNSTABLE_DIFFUSION_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <cstdint>
#include <ostream>
#include <string_view>
#include <unordered_set>
#include <vector>

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

struct Field {
    std::unordered_set<Point, HashPoint> points;
    Point min;
    Point max;
    bool did_change;
};

template<>
struct fmt::formatter<Field>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Field const& f, FormatContext& ctx) {
        for (int iy = f.min.y; iy < f.max.y; ++iy) {
            for (int ix = f.min.x; ix < f.max.x; ++ix) {
                char const c = (f.points.contains(Point{ .x = ix, .y = iy }) ? '#' : '.');
                fmt::format_to(ctx.out(), "{}", c);
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};


Field parseInput(std::string_view input);

Field move(Field const& f0, int step_count);

int answer1(Field const& field);

int answer2(Field const& field);

#endif
