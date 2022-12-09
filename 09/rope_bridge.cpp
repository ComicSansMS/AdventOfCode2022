#include <rope_bridge.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/format.h>

#include <cassert>
#include <regex>

std::vector<Instruction> parseInput(std::string_view input)
{
    std::vector<Instruction> ret;
    std::regex rx_line(R"((\w) (\d+))");
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(ranges::begin(input), ranges::end(input), rx_line);
    auto const it_end = regex_it();
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> Instruction {
            Direction const direction = [](auto d) {
                switch (*d.first) {
                default: assert(!"Invalid direction"); [[fallthrough]];
                case 'L': return Direction::Left;
                case 'R': return Direction::Right;
                case 'U': return Direction::Up;
                case 'D': return Direction::Down;
                }
            }(match[1]);
            return Instruction{
                .direction = direction,
                .amount = std::stoi(match[2])
            };
        });
    return ret;
}

template<std::size_t N>
std::int64_t answer(std::vector<Instruction> const& instructions)
{
    Rope<N> r;
    for (auto const& instruction : instructions) {
        moveHead(r, instruction);
    }
    return static_cast<std::int64_t>(r.visited.size());
}

std::int64_t answer1(std::vector<Instruction> const& instructions)
{
    return answer<2>(instructions);
}

std::int64_t answer2(std::vector<Instruction> const& instructions)
{
    return answer<10>(instructions);
}
