#include <supply_stacks.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/reverse.hpp>
#include <range/v3/view/split.hpp>

#include <cassert>
#include <optional>
#include <regex>
#include <string>
#include <tuple>

Input parseInput(std::string_view input)
{
    Input ret;
    bool last_stack_line = false;
    std::string_view::iterator it_start_instructions;
    Field& field = ret.field;
    for (auto l : input | ranges::views::split('\n')) {
        std::string const line = l | ranges::to<std::string>;
        if (line.empty()) {
            char const* const current_offset = &(*ranges::begin(l));
            it_start_instructions = ranges::begin(input) + (current_offset - &input[0] + 1);
            break;
        }
        if (field.stacks.empty()) {
            assert((line.size() + 1) % 4 == 0);
            auto const n_stacks = (line.size() + 1) / 4;
            field.stacks.resize(n_stacks);
        } else {
            assert(line.size() == (field.stacks.size() * 4) - 1);
        }
        for (std::size_t i = 0, i_end = field.stacks.size(); i != i_end; ++i) {
            char const c = line[(i * 4) + 1];
            if (std::isdigit(c)) {
                last_stack_line = true;
                assert(static_cast<std::size_t>(c - '0') == i + 1);
            } else if (c == ' ') {
                // empty place on stack
                assert(field.stacks[i].empty());
                assert(!last_stack_line);
            } else {
                assert(!last_stack_line);
                field.stacks[i].push_back(c);
            }
        }
    }
    for (auto& s : field.stacks) { ranges::reverse(s); }

    std::regex rx_line(R"(move (\d+) from (\d+) to (\d+))");
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(it_start_instructions, ranges::end(input), rx_line);
    auto const it_end = regex_it();
    std::transform(it_begin, it_end, std::back_inserter(ret.instructions),
        [](std::match_results<std::string_view::iterator> const& match) -> Instruction {
            return Instruction{
                .amount = std::stoi(match[1]),
                .from = std::stoi(match[2]),
                .to = std::stoi(match[3])
            };
        });

    return ret;
}

void executeInstruction(Field& f, Instruction const& i)
{
    for (int j = 0; j < i.amount; ++j) {
        char const c = f.stacks[i.from - 1].back();
        f.stacks[i.from - 1].pop_back();
        f.stacks[i.to - 1].push_back(c);
    }
}

std::string getOutput(Field const& f)
{
    std::string ret;
    for (auto const& s : f.stacks) {
        ret.push_back(s.back());
    }
    return ret;
}

std::string answer1(Input const& in)
{
    Field f = in.field;
    for (auto const& i : in.instructions) {
        executeInstruction(f, i);
    }
    return getOutput(f);
}

void executeInstruction9001(Field& f, Instruction const& i)
{
    auto& v_to = f.stacks[i.to - 1];
    auto& v_from = f.stacks[i.from - 1];
    auto it_from = ranges::begin(v_from) + (ranges::size(v_from) - i.amount);
    v_to.insert(ranges::end(v_to), it_from, ranges::end(v_from));
    v_from.erase(it_from, ranges::end(v_from));
}

std::string answer2(Input const& in)
{
    Field f = in.field;
    for (auto const& i : in.instructions) {
        executeInstruction9001(f, i);
    }
    return getOutput(f);
}
