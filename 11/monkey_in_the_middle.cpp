#include <monkey_in_the_middle.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/partial_sort.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <cassert>
#include <concepts>
#include <cmath>
#include <string>

std::vector<Monkey> parseInput(std::string_view input)
{
    std::vector<Monkey> ret;
    enum ParserState {
        ExpectMonkey = 0,
        ExpectItems,
        ExpectOperation,
        ExpectTest,
        ExpectBranchTrue,
        ExpectBranchFalse,
        ExpectNewLine
    } state = ParserState::ExpectMonkey;
    auto const advanceState = [](ParserState& s) {
        s = (s == ParserState::ExpectNewLine) ? 
            ParserState::ExpectMonkey :
            static_cast<ParserState>(static_cast<int>(s) + 1);
    };
    for (auto line : input | ranges::views::split('\n')) {
        std::string const l = line | ranges::to<std::string>;
        switch (state) {
        case ParserState::ExpectMonkey:
            assert(l.starts_with("Monkey "));
            assert(l.ends_with(":"));
            assert(std::stoi(l.substr(7, 1)) == static_cast<int>(ret.size()));
            ret.emplace_back();
            ret.back().inspection_count = 0;
            advanceState(state);
            break;
        case ParserState::ExpectItems:
            assert(l.starts_with("  Starting items: "));
            ret.back().items = l
                | ranges::views::drop(17)
                | ranges::views::split(',')
                | ranges::views::transform([](auto sn) -> std::int64_t
                                           { return std::stoi(ranges::to<std::string>(sn)); })
                | ranges::to<std::vector>;
            advanceState(state);
            break;
        case ParserState::ExpectOperation:
            assert(l.starts_with("  Operation: "));
            {
                char const c_op = l[23];
                if (c_op == '+') {
                    ret.back().operation = Operation{ .op = Op::Add, .arg = std::stoi(l.substr(25)) };
                } else {
                    assert(c_op == '*');
                    if (l[25] == 'o') {
                        ret.back().operation = Operation{ .op = Op::Sqr, .arg = 0 };
                    } else {
                        ret.back().operation = Operation{ .op = Op::Mul, .arg = std::stoi(l.substr(25)) };
                    }
                }
            }
            advanceState(state);
            break;
        case ParserState::ExpectTest:
            assert(l.starts_with("  Test: divisible by "));
            ret.back().test_divisor = std::stoi(l.substr(21));
            advanceState(state);
            break;
        case ParserState::ExpectBranchTrue:
            assert(l.starts_with("    If true: throw to monkey "));
            ret.back().monkey_true = std::stoi(l.substr(29));
            advanceState(state);
            break;
        case ParserState::ExpectBranchFalse:
            assert(l.starts_with("    If false: throw to monkey "));
            ret.back().monkey_false = std::stoi(l.substr(30));
            advanceState(state);
            break;
        case ParserState::ExpectNewLine:
            assert(l.empty());
            advanceState(state);
            break;
        }
    }
    return ret;
}

std::int64_t executeOp(Operation const& op, std::int64_t item) {
    if (op.op == Op::Add) {
        return item + op.arg;
    } else if (op.op == Op::Mul) {
        return item * op.arg;
    } else {
        assert(op.op == Op::Sqr);
        return item * item;
    }
}

template<std::regular_invocable<std::int64_t> Func_T>
void playTurn(std::vector<Monkey>& monkeys, std::size_t active_monkey_index, Func_T&& decay_func)
{
    assert((active_monkey_index >= 0) && (active_monkey_index < monkeys.size()));
    auto& monkey = monkeys[active_monkey_index];
    for (auto const& item : monkey.items) {
        auto const new_item = decay_func(executeOp(monkey.operation, item));
        auto const target_index = ((new_item % monkey.test_divisor) == 0) ? monkey.monkey_true : monkey.monkey_false;
        monkeys[target_index].items.push_back(new_item);
    }
    monkey.inspection_count += monkey.items.size();
    monkey.items.clear();
}

void playTurn(std::vector<Monkey>& monkeys, std::size_t active_monkey_index)
{
    playTurn(monkeys, active_monkey_index, [](std::int64_t i) { return i / 3; });
}

void playRound(std::vector<Monkey>& monkeys)
{
    for (std::size_t i = 0, i_end = monkeys.size(); i != i_end; ++i) {
        playTurn(monkeys, i);
    }
}

void playRounds(std::vector<Monkey>& monkeys, int count)
{
    for (int i = 0; i < count; ++i) {
        playRound(monkeys);
    }
}

std::int64_t scoreMonkeyBusiness(std::vector<Monkey> m)
{
    ranges::partial_sort(m, m.begin() + 2,
        [](Monkey const& lhs, Monkey const& rhs) -> bool { return lhs.inspection_count >= rhs.inspection_count; });
    return m[0].inspection_count * m[1].inspection_count;
}

std::int64_t answer1(std::vector<Monkey> const& monkeys)
{
    auto m = monkeys;
    playRounds(m, 20);
    return scoreMonkeyBusiness(m);
}

std::int64_t determineNeutralDecay(std::vector<Monkey> const& monkeys)
{
    return ranges::accumulate(monkeys, 1, ranges::multiplies{}, [](Monkey const& m) { return m.test_divisor; });
}

void playTurn(std::vector<Monkey>& monkeys, std::size_t active_monkey_index, std::int64_t neutral_decay)
{
    playTurn(monkeys, active_monkey_index, [neutral_decay](std::int64_t i) { return i % neutral_decay; });
}

void playRound(std::vector<Monkey>& monkeys, std::int64_t neutral_decay)
{
    for (std::size_t i = 0, i_end = monkeys.size(); i != i_end; ++i) {
        playTurn(monkeys, i, neutral_decay);
    }
}

void playRounds(std::vector<Monkey>& monkeys, int count, std::int64_t neutral_decay)
{
    for (int i = 0; i < count; ++i) {
        playRound(monkeys, neutral_decay);
    }
}

std::int64_t answer2(std::vector<Monkey> const& monkeys)
{
    auto m = monkeys;
    playRounds(m, 10000, determineNeutralDecay(m));
    return scoreMonkeyBusiness(m);
}
