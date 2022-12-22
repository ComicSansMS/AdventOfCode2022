#ifndef ADVENT_OF_CODE_22_MONKEY_MAP_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_22_MONKEY_MAP_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

enum class Cell {
    Void = 0,
    Empty,
    Wall
};

template<>
struct fmt::formatter<Cell>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Cell c, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "{}", ((c == Cell::Void) ? ' ' : ((c == Cell::Empty) ? '.' : '#')));
    }
};

struct Range {
    int first;
    int last;

    friend bool operator==(Range, Range) = default;
};

struct Map {
    std::vector<Cell> data;
    int width;
    int height;
    std::vector<Range> row_width;
    std::vector<Range> col_height;
};

enum class Turn {
    Left,
    Right
};

struct Forward {
    int amount;

    friend bool operator==(Forward, Forward) = default;
};

using Step = std::variant<Turn, Forward>;

using Path = std::vector<Step>;

struct Input {
    Map map;
    Path path;
};

inline auto mapSpan(Map const& m) noexcept {
    return std::experimental::mdspan<Cell const, std::experimental::dextents<int, 2>, std::experimental::layout_left>(m.data.data(), m.width, m.height);
}

template<>
struct fmt::formatter<Map>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Map const& m, FormatContext& ctx) {
        auto const mds = mapSpan(m);
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

Input parseInput(std::string_view input);

enum class Direction {
    Right = 0,
    Down,
    Left,
    Up
};

Direction turnLeft(Direction d);
Direction turnRight(Direction d);

struct Position {
    int x;
    int y;
    Direction dir;

    friend bool operator==(Position, Position) = default;
};

Position determineStartingPosition(Map const& m);

Position walk(Position const& start, Map const& m, Step const& s);

int answer1(Input const& in);

Position walkCube(Position const& start, Map const& m, Step const& s);

int answer2(Input const& in);


#endif
