#ifndef ADVENT_OF_CODE_10_CATHODE_RAY_TUBE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_10_CATHODE_RAY_TUBE_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <experimental/mdspan>

#include <array>
#include <cstdint>
#include <string_view>
#include <variant>
#include <vector>

struct Noop {
    friend bool operator==(Noop, Noop) = default;
};

struct AddX {
    std::int64_t val;

    friend bool operator==(AddX, AddX) = default;
};

using Instruction = std::variant<AddX, Noop>;

std::vector<Instruction> parseInput(std::string_view input);

std::vector<std::int64_t> computeValuesX(std::vector<Instruction> const& instructions);

std::int64_t answer1(std::vector<Instruction> const& instructions);

struct CRT {
    std::array<char, 40*6> screen;

    CRT();
};

template<>
struct fmt::formatter<CRT>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(CRT const& crt, FormatContext& ctx) {
        std::experimental::mdspan mds(crt.screen.data(), std::experimental::extents<int, 40, 6>{});
        for (int iy = 0; iy < mds.extent(1); ++iy) {
            for (int ix = 0; ix < mds.extent(0); ++ix) {
                fmt::format_to(ctx.out(), "{}", mds(ix, iy));
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

CRT answer2(std::vector<Instruction> const& instructions);

#endif
