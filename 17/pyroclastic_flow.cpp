#include <pyroclastic_flow.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <regex>


std::vector<Instruction> parseInput(std::string_view input)
{
    auto char_to_instr = [](char c) -> Instruction {
        assert((c == '>') || (c == '<'));
        return (c == '>') ? Instruction::Right : Instruction::Left;
    };;
    return input
        | ranges::views::take_while([](char c) { return c != '\n'; })
        | ranges::views::transform(char_to_instr)
        | ranges::to<std::vector>;
}

RockShape nextShape(RockShape s)
{
    return static_cast<RockShape>((static_cast<int>(s) + 1) % 5);
}

Rock const& getRock(RockShape s)
{
    static std::array<Rock, 5> rocks = {
        Rock {
            .shape = RockShape::Horiz,
            .cells = { Cell::Rock, Cell::Rock, Cell::Rock, Cell::Rock },
            .width = 4,
            .height = 1,
        },
        Rock {
            .shape = RockShape::Plus,
            .cells = { Cell::Empty, Cell::Rock, Cell::Empty,
                       Cell::Rock,  Cell::Rock, Cell::Rock,
                       Cell::Empty, Cell::Rock, Cell::Empty },
            .width = 3,
            .height = 3,
        },
        Rock {
            .shape = RockShape::L,
            .cells = { Cell::Empty, Cell::Empty, Cell::Rock,
                       Cell::Empty, Cell::Empty, Cell::Rock,
                       Cell::Rock,  Cell::Rock,  Cell::Rock },
            .width = 3,
            .height = 3,
        },
        Rock {
            .shape = RockShape::Vert,
            .cells = { Cell::Rock, Cell::Rock, Cell::Rock, Cell::Rock },
            .width = 1,
            .height = 4
        },
        Rock {
            .shape = RockShape::Square,
            .cells = { Cell::Rock, Cell::Rock, Cell::Rock, Cell::Rock },
            .width = 2,
            .height = 2
        },
    };
    return rocks[static_cast<int>(s)];
}

bool doesCollide(Field const& f, Rock const& r, Point const& p)
{
    std::experimental::mdspan mds(r.cells.data(), r.width, r.height);
    Point const p_ul{ .x = p.x, .y = p.y + r.height };
    for (int iy = 0; iy < static_cast<int>(mds.extent(1)); ++iy) {
        for (int ix = 0; ix < static_cast<int>(mds.extent(0)); ++ix) {
            if (mds(ix, iy) == Cell::Rock) {
                Point p_check{ .x = p_ul.x + ix, .y = p_ul.y - iy };
                if ((p_check.x < 0) || (p_check.x >= 7) || (p_check.y <= 0)) { return true; }
                if (f.rock_cells.contains(p_check)) { return true; }
            }
        }
    }
    return false;
}

void placeRock(Field& f, Rock const& r, Point const& p)
{
    std::experimental::mdspan mds(r.cells.data(), r.width, r.height);
    Point const p_ul{ .x = p.x, .y = p.y + r.height };
    for (int iy = 0; iy < static_cast<int>(mds.extent(1)); ++iy) {
        for (int ix = 0; ix < static_cast<int>(mds.extent(0)); ++ix) {
            if (mds(ix, iy) == Cell::Rock) {
                Point p_place{ .x = p_ul.x + ix, .y = p_ul.y - iy };
                assert((p_place.x >= 0) && (p_place.x < 7) && (p_place.y > 0) && (!f.rock_cells.contains(p_place)));
                f.rock_cells.insert(p_place);
            }
        }
    }
}

void spawnRock(Field& f, std::vector<Instruction> const& instructions)
{
    Rock const& r = getRock(f.next_rock);
    f.next_rock = nextShape(f.next_rock);
    Point pos{ .x = 2, .y = f.max_y + 3 };
    for (;;) {
        auto const instr = instructions[f.simIp];
        ++f.simIp;
        if (f.simIp == static_cast<int>(instructions.size())) { f.simIp = 0; }
        Point new_pos = pos;
        new_pos.x = new_pos.x + ((instr == Instruction::Left) ? -1 : 1);
        if (doesCollide(f, r, new_pos)) {
            new_pos.x = pos.x;
        }
        --new_pos.y;
        if (doesCollide(f, r, new_pos)) {
            placeRock(f, r, Point{ new_pos.x, pos.y });
            f.max_y = std::max(f.max_y, pos.y + r.height);
            break;
        }
        pos = new_pos;
    }
}

int answer1(std::vector<Instruction> const& instructions)
{
    Field f;
    for (int i = 0; i < 2022; ++i) {
        spawnRock(f, instructions);
    }
    return f.max_y;
}

bool isSolidLine(Field const& f, int y)
{
    std::string str;
    for (int ix = 0; ix < 7; ++ix) {
        Point p{ .x = ix, .y = y };
        str.push_back(f.rock_cells.contains(p) ? '#' : '.');
    }
    if (str == "#######") { return true; }
    str = ".......";
    for (int ix = 0; ix < 7; ++ix) {
        if (f.rock_cells.contains(Point{ .x = ix, .y = y }) ||
            f.rock_cells.contains(Point{ .x = ix, .y = y - 1 }) ||
            f.rock_cells.contains(Point{ .x = ix, .y = y - 2 }) ||
            f.rock_cells.contains(Point{ .x = ix, .y = y - 3 })) {
            str[ix] = '#';
        }
    }
    if (str == "#######") { return true; }
    return false;
}

/** This is still somewhat buggy and introduces off-by-one errors for certain inputs
 */
int64_t answer2(std::vector<Instruction> const& instructions, bool off_by_one)
{
    int64_t const target = 1000000000000;
    Field f;
    std::int64_t i = 0;
    for (; i < 2022; ++i) {
        spawnRock(f, instructions);
    }

    for (;; ++i) {
        spawnRock(f, instructions);
        if (isSolidLine(f, f.max_y)) { break; }
    }

    int const pattern_height = 25;
    std::array<Cell, 7 * pattern_height> base_pattern;
    std::experimental::mdspan mds(base_pattern.data(), 7, pattern_height);
    for (int iy = 0; iy < pattern_height; ++iy) {
        int const field_y = f.max_y - iy;
        for (int ix = 0; ix < 7; ++ix) {
            Point p{ .x = ix, .y = field_y };
            mds(ix, iy) = f.rock_cells.contains(p) ? Cell::Rock : Cell::Empty;
        }
    }
    auto checkPattern = [mds](Field const& f) -> bool {
        for (int iy = 0; iy < pattern_height; ++iy) {
            int const field_y = f.max_y - iy;
            for (int ix = 0; ix < 7; ++ix) {
                Point p{ .x = ix, .y = field_y };
                bool field_has_rock = f.rock_cells.contains(p);
                bool pattern_has_rock = (mds(ix, iy) == Cell::Rock);
                if (field_has_rock != pattern_has_rock) { return false; }
            }
        }
        return true;
    };

    int const base_y = f.max_y;
    int const base_ip = f.simIp;
    int64_t const base_i = i;
    for (; ; ++i) {
        spawnRock(f, instructions);
        if (checkPattern(f)) { break; }
    }

    assert(base_ip == f.simIp);
    int64_t const d_i = (i - base_i) + 1;
    int64_t const d_y = f.max_y - base_y;

    int64_t const cycles_skipped = (target - i) / d_i;
    int64_t const lines_skipped = cycles_skipped * d_y;
    int64_t const rest = (target - i) % d_i;
    i += cycles_skipped * d_i;
    assert(i + rest == target);
    for (; i < target + (off_by_one ? 1 : 0); ++i) {
        spawnRock(f, instructions);
    }

     return static_cast<int64_t>(f.max_y) + lines_skipped - 4;
}
