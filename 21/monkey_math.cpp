#include <monkey_math.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>


#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <tuple>

Operation::Operator parseOperator(char op)
{
    if (op == '+') {
        return Operation::Plus;
    } else if (op == '-') {
        return Operation::Minus;
    } else if (op == '*') {
        return Operation::Multiply;
    } else {
        assert(op == '/');
        return Operation::Divide;
    }
}

std::unordered_map<std::string, Monkey> parseInput(std::string_view input)
{
    auto parseLine = [](auto v) -> Monkey {
        auto s = ranges::to<std::string>(v);
        std::string name = s.substr(0, 4);
        assert(s[4] == ':');
        assert(s[5] == ' ');
        if ((s[6] == '-') || (std::isdigit(s[6]))) {
            auto const number_str = s.substr(6);
            return Monkey{ .name = name, .value = std::stoi(number_str), .op = std::nullopt };
        } else {
            Operation const operation{ .op = parseOperator(s[11]), .lhs = s.substr(6, 4), .rhs = s.substr(13, 4) };
            return Monkey{ .name = name, .value = std::nullopt, .op = operation };
        }
    };
    auto toPair = [](Monkey&& m) -> std::pair<std::string, Monkey> {
        return std::make_pair(m.name, std::move(m));
    };
    return input | ranges::views::split('\n') | ranges::views::transform(parseLine) | ranges::views::transform(toPair) | ranges::to<std::unordered_map>;
}

// LCOV_EXCL_START
std::string toDot(std::unordered_map<std::string, Monkey> const& monkeys)
{
    auto opString = [](Operation::Operator op) {
        if (op == Operation::Plus) {
            return "+";
        } else if (op == Operation::Minus) {
            return "-";
        } else if (op == Operation::Multiply) {
            return "*";
        } else {
            assert(op == Operation::Divide);
            return "/";
        }
    };
    std::string ret;
    ret = "digraph D {" "\n";
    for (auto const& [name, m] : monkeys) {
        if (m.value) {
            ret += fmt::format(R"(  {} [shape=box, label="{}\n{}"])" "\n", name, name, *m.value);
        } else {
            ret += fmt::format(R"(  {} [shape=box, label="{}\n{}"])" "\n", name, name, opString(m.op->op));
        }
    }
    ret += "\n";
    for (auto const& [name, m] : monkeys) {
        if (m.op) {
            ret += fmt::format("  {} -> {}\n", name, m.op->lhs);
            ret += fmt::format("  {} -> {}\n", name, m.op->rhs);
        }
    }
    ret += "}\n";
    return ret;
}
// LCOV_EXCL_STOP

int64_t executeOp(int64_t lhs, Operation::Operator op, int64_t rhs)
{
    if (op == Operation::Plus) {
        return lhs + rhs;
    } else if (op == Operation::Minus) {
        return lhs - rhs;
    } else if (op == Operation::Multiply) {
        return lhs * rhs;
    } else {
        assert(op == Operation::Divide);
        return lhs / rhs;
    }
}

int64_t eval(std::unordered_map<std::string, Monkey>& monkeys, std::string const& node)
{
    assert(monkeys.contains(node));
    std::vector<Monkey*> monkeystack;
    monkeystack.push_back(&monkeys[node]);
    while (!monkeystack.empty()) {
        Monkey* m = monkeystack.back();
        if (m->value) {
            monkeystack.pop_back();
        } else {
            assert(m->op);
            Operation const& op = *m->op;
            Monkey& lhs = monkeys[op.lhs];
            Monkey& rhs = monkeys[op.rhs];
            if ((!lhs.value) || (!rhs.value)) {
                if (!lhs.value) {
                    monkeystack.push_back(&lhs);
                }
                if (!rhs.value) {
                    monkeystack.push_back(&rhs);
                }
            } else {
                m->value = executeOp(*lhs.value, op.op, *rhs.value);
            }
        }
    }
    assert(monkeys[node].value.has_value());
    return monkeys[node].value.value();
}

int64_t answer1(std::unordered_map<std::string, Monkey> monkeys)
{
    return eval(monkeys, "root");;
}

enum class Turn : std::int8_t {
    L,
    R
};

std::optional<std::vector<Turn>> findHuman(std::unordered_map<std::string, Monkey>& monkeys, std::string const& node)
{
    assert(monkeys.contains(node));
    struct StackElement {
        Monkey* m;
        std::vector<Turn> path;
    };
    std::vector<StackElement> monkeystack;
    monkeystack.push_back(StackElement{ .m = &monkeys[node], .path = {} });
    while (!monkeystack.empty()) {
        auto [m, path] = monkeystack.back();
        monkeystack.pop_back();
        if (m->name == "humn") { return path; }
        if (m->op) {
            Operation const& op = *m->op;
            monkeystack.push_back(StackElement{ .m = &monkeys[op.lhs], .path = path });
            monkeystack.back().path.push_back(Turn::L);
            monkeystack.push_back(StackElement{ .m = &monkeys[op.rhs], .path = path });
            monkeystack.back().path.push_back(Turn::R);
        }
    }
    return std::nullopt;
}

int64_t reduceNumber(int64_t n, Operation::Operator op, int64_t operand, bool operand_is_left)
{
    if (op == Operation::Plus) {
        return n - operand;
    } else if (op == Operation::Minus) {
        if (operand_is_left) {
            return operand - n;
        } else {
            return n + operand;
        }
    } else if (op == Operation::Multiply) {
        return n / operand;
    } else {
        assert(op == Operation::Divide);
        if (operand_is_left) {
            return operand / n;
        } else {
            return n * operand;
        }
    }
}

int64_t determineHumanNumber(std::unordered_map<std::string, Monkey>& monkeys, std::string const& node, std::vector<Turn> const& path, int64_t number_to_match)
{
    Monkey* it_node = &monkeys[node];
    int64_t it_number = number_to_match;
    auto it_turn = path.begin();
    while (it_node->name != "humn") {
        assert(it_node->op);
        Operation const& op = it_node->op.value();
        Turn const turn = *it_turn;
        int64_t other;
        if (turn == Turn::L) {
            other = eval(monkeys, op.rhs);
            it_node = &monkeys[op.lhs];
            it_number = reduceNumber(it_number, op.op, other, false);
        } else {
            assert(turn == Turn::R);
            other = eval(monkeys, op.lhs);
            it_node = &monkeys[op.rhs];
            it_number = reduceNumber(it_number, op.op, other, true);
        }
        ++it_turn;
    }
    return it_number;
}

int64_t answer2(std::unordered_map<std::string, Monkey> monkeys)
{
    Monkey const& root = monkeys["root"];
    std::string const& s_lhs = root.op->lhs;
    std::string const& s_rhs = root.op->rhs;
    auto lhs_path_human = findHuman(monkeys, s_lhs);
    auto rhs_path_human = findHuman(monkeys, s_rhs);
    bool lhs_has_human = lhs_path_human.has_value();
    bool rhs_has_human = rhs_path_human.has_value();
    assert((lhs_has_human || rhs_has_human) && (!(lhs_has_human && rhs_has_human)));
    std::string const s_humn = (lhs_has_human ? s_lhs : s_rhs);
    auto const& p_humn = (lhs_has_human ? lhs_path_human : rhs_path_human).value();
    std::string const s_nmbr = (lhs_has_human ? s_rhs : s_lhs);

    int64_t const number_to_match = eval(monkeys, s_nmbr);
    int64_t const human_number = determineHumanNumber(monkeys, s_humn, p_humn, number_to_match);
    monkeys["humn"].value = human_number;
    int64_t const number_produced = eval(monkeys, s_humn);
    assert(number_produced == number_to_match);

    return human_number;
}
