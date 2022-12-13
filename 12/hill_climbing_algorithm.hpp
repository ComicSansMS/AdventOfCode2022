#ifndef ADVENT_OF_CODE_12_HILL_CLIMBING_ALGORITHM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_12_HILL_CLIMBING_ALGORITHM_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <cstdint>
#include <string_view>
#include <ostream>
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

using MapSpan = std::experimental::mdspan<int, std::experimental::dextents<int, 2>, std::experimental::layout_left>;

struct Map {
    std::vector<int> data;
    MapSpan map;

    Point start;
    Point end;
};

Map parseInput(std::string_view input);

struct AllPaths {
    std::vector<int> data;
    MapSpan distances;
};

AllPaths allPaths(Map const& m, std::vector<Point> const& start_points);

AllPaths allPathsFromStart(Map const& m);

int answer1(Map const& m);

AllPaths allPathsFromA(Map const& m);

int answer2(Map const& m);

#endif
