#ifndef ADVENT_OF_CODE_24_BLIZZARD_BASIN_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_24_BLIZZARD_BASIN_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <array>
#include <bit>
#include <cassert>
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
    int z;                                                  // LCOV_EXCL_LINE

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
        return fmt::format_to(ctx.out(), "({}, {}, {})", p.x, p.y, p.z);
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
        hash_combine(seed, v.z);
        return seed;
    }
};

enum class Cell : unsigned int {
    Empty = 0,
    Up    = 0x1,
    Down  = 0x2,
    Left  = 0x4,
    Right = 0x8,
};

inline int cellCount(Cell c)
{
    return std::popcount(static_cast<unsigned int>(c));
}

template<>
struct fmt::formatter<Cell>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Cell c, FormatContext& ctx) {
        if (c == Cell::Empty) {
            return fmt::format_to(ctx.out(), ".");
        } else if (c == Cell::Up) {
            return fmt::format_to(ctx.out(), "^");
        } else if (c == Cell::Down) {
            return fmt::format_to(ctx.out(), "v");
        } else if (c == Cell::Left) {
            return fmt::format_to(ctx.out(), "<");
        } else if (c == Cell::Right) {
            return fmt::format_to(ctx.out(), ">");
        } else {
            return fmt::format_to(ctx.out(), "{}", cellCount(c));
        }
    }
};


struct Field {
    std::vector<Cell> map;
    int width;
    int height;
};

inline auto fieldSpan(Field const& f) noexcept {
    return std::experimental::mdspan<Cell const, std::experimental::dextents<int, 2>, std::experimental::layout_left>(f.map.data(), f.width, f.height);
}

inline auto fieldSpan(Field& f) noexcept {
    return std::experimental::mdspan<Cell, std::experimental::dextents<int, 2>, std::experimental::layout_left>(f.map.data(), f.width, f.height);
}


template<>
struct fmt::formatter<Field>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Field const& f, FormatContext& ctx) {
        auto const mds = fieldSpan(f);
        for (int iy = 0; iy < mds.extent(1); ++iy) {
            for (int ix = 0; ix < mds.extent(0); ++ix) {
                Cell const c = mds(ix, iy);
                fmt::format_to(ctx.out(), "{}", c);
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

Field parseInput(std::string_view input);

Field advanceTime(Field const& f);

struct FieldCube {
    std::vector<Cell> map;
    int width;
    int height;
    int depth;
};

inline auto cubeSpan(FieldCube const& f) noexcept {
    return std::experimental::mdspan<Cell const, std::experimental::dextents<int, 3>, std::experimental::layout_left>(f.map.data(), f.width, f.height, f.depth);
}

inline auto cubeSpan(FieldCube& f) noexcept {
    return std::experimental::mdspan<Cell, std::experimental::dextents<int, 3>, std::experimental::layout_left>(f.map.data(), f.width, f.height, f.depth);
}

FieldCube buildFieldCube(Field const& field);

int solve(FieldCube const& c, Point const& start, Point const& goal);

int answer1(FieldCube const& c);

int answer2(FieldCube const& c);

#endif
