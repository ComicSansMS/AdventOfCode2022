#ifndef ADVENT_OF_CODE_11_MONKEY_IN_THE_MIDDLE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_11_MONKEY_IN_THE_MIDDLE_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

enum class Op {
    Add,
    Mul,
    Sqr
};

struct Operation {
    Op op;
    std::int64_t arg;

    friend bool operator==(Operation, Operation) = default;
};

struct Monkey {
    std::vector<std::int64_t> items;
    Operation operation;
    std::int64_t test_divisor;
    int monkey_true;
    int monkey_false;

    int64_t inspection_count;
};

std::vector<Monkey> parseInput(std::string_view input);

void playTurn(std::vector<Monkey>& monkeys, std::size_t active_monkey_index);

void playRound(std::vector<Monkey>& monkeys);

void playRounds(std::vector<Monkey>& monkeys, int counts);

std::int64_t answer1(std::vector<Monkey> const& monkeys);

std::int64_t determineNeutralDecay(std::vector<Monkey> const& monkeys);

void playTurn(std::vector<Monkey>& monkeys, std::size_t active_monkey_index, std::int64_t neutral_decay);

void playRound(std::vector<Monkey>& monkeys, std::int64_t neutral_decay);

void playRounds(std::vector<Monkey>& monkeys, int counts, std::int64_t neutral_decay);

std::int64_t answer2(std::vector<Monkey> const& monkeys);

#endif
