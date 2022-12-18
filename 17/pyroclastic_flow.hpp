#ifndef ADVENT_OF_CODE_17_PYROCLASTIC_FLOW_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_17_PYROCLASTIC_FLOW_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

enum class Instruction {
    Left,
    Right
};

enum class Cell {
    Empty,
    Rock
};

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

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct HashPoint {
    inline std::size_t operator()(Point const& v) const {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        return seed;
    }
};

enum class RockShape : int {
    Horiz = 0,
    Plus,
    L,
    Vert,
    Square
};

struct Rock {
    RockShape shape;
    std::vector<Cell> cells;
    int width;
    int height;
};

RockShape nextShape(RockShape s);

Rock const& getRock(RockShape s);

struct Field {
    int max_y = 0;
    RockShape next_rock = RockShape::Horiz;
    std::unordered_set<Point, HashPoint> rock_cells;
    int simIp = 0;

    Field() = default;
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
        for (int iy = f.max_y; iy > 0; --iy) {
            fmt::format_to(ctx.out(), "|");
            for (int ix = 0; ix < 7; ++ix) {
                if (f.rock_cells.contains(Point{ ix, iy })) {
                    fmt::format_to(ctx.out(), "#");
                } else {
                    fmt::format_to(ctx.out(), ".");
                }
            }
            fmt::format_to(ctx.out(), "|\n");
        }
        fmt::format_to(ctx.out(), "+-------+\n");
        return ctx.out();
    }
};

std::vector<Instruction> parseInput(std::string_view input);

void spawnRock(Field& f, std::vector<Instruction> const& instructions);

int answer1(std::vector<Instruction> const& instructions);

int64_t answer2(std::vector<Instruction> const& instructions, bool off_by_one = false);

#endif
