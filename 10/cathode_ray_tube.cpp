#include <cathode_ray_tube.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/format.h>

#include <cassert>
#include <cmath>
#include <string>

std::vector<Instruction> parseInput(std::string_view input)
{
    std::string s;
    return input | ranges::views::split('\n') | ranges::views::transform([](auto line) -> Instruction {
            auto l = line | ranges::to<std::string>;
            if (l == "noop") {
                return Noop{};
            } else {
                assert(l.starts_with("addx "));
                return AddX{ .val = std::stoi(l.substr(5)) };
            }
        }) | ranges::to<std::vector>;
}

struct OpResult {
    std::int64_t d_ip;
    std::int64_t d_x;
};

struct ExecuteInstructionVisitor {
    OpResult operator()(Noop) {
        return OpResult{ .d_ip = 1, .d_x = 0 };
    }

    OpResult operator()(AddX add) {
        return OpResult{ .d_ip = 2, .d_x = add.val };
    }
};

std::vector<std::int64_t> computeValuesX(std::vector<Instruction> const& instructions)
{
    std::vector<std::int64_t> xs;
    xs.push_back(1);
    xs.push_back(1);
    for (auto const& op : instructions) {
        auto const [d_ip, d_x] = std::visit(ExecuteInstructionVisitor{}, op);
        for (int i = 1; i < d_ip; ++i) { xs.push_back(xs.back()); }
        xs.push_back(xs.back() + d_x);
    }
    return xs;
}

std::int64_t signalStrength(std::vector<std::int64_t> const& xs, std::int64_t cycle)
{
    return cycle * xs[cycle];
}

std::int64_t answer1(std::vector<Instruction> const& instructions)
{
    auto const xs = computeValuesX(instructions);
    std::int64_t const indices[] = { 20, 60, 100, 140, 180, 220 };
    return ranges::accumulate(indices, std::int64_t{ 0 }, ranges::plus{},
        [&xs](std::size_t cycle) -> std::int64_t { return signalStrength(xs, cycle); });
}

CRT::CRT()
{
    screen.fill('?');
}

CRT answer2(std::vector<Instruction> const& instructions)
{
    CRT crt;
    std::experimental::mdspan mds(crt.screen.data(), std::experimental::extents<int, 40, 6>{});
    int cycle_count = 1;
    auto const xs = computeValuesX(instructions);
    for (int iy = 0; iy < mds.extent(1); ++iy) {
        for (int ix = 0; ix < mds.extent(0); ++ix, ++cycle_count) {
            auto const sprite_pos = xs[cycle_count];
            char const c = (std::abs(sprite_pos - ix) <= 1) ? '#' : '.';
            mds(ix, iy) = c;
        }
    }
    return crt;
}
