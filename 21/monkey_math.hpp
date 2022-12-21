#ifndef ADVENT_OF_CODE_21_MONKEY_MATH_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_21_MONKEY_MATH_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Operation {
    enum Operator {
        Plus, Minus, Multiply, Divide
    } op;
    std::string lhs;
    std::string rhs;
};

struct Monkey {
    std::string name;
    std::optional<int64_t> value;
    std::optional<Operation> op;
};

std::unordered_map<std::string, Monkey> parseInput(std::string_view input);

std::string toDot(std::unordered_map<std::string, Monkey> const& monkeys);

int64_t eval(std::unordered_map<std::string, Monkey>& monkeys, std::string const& node);

int64_t answer1(std::unordered_map<std::string, Monkey> monkeys);

int64_t answer2(std::unordered_map<std::string, Monkey> monkeys);

#endif
