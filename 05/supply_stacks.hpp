#ifndef ADVENT_OF_CODE_05_SUPPLY_STACKS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_05_SUPPLY_STACKS_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

using Stack = std::vector<char>;

struct Instruction {
    int amount;
    int from;
    int to;

    friend bool operator==(Instruction const&, Instruction const&) = default;
};

struct Field {
    std::vector<Stack> stacks;
};

struct Input {
    Field field;
    std::vector<Instruction> instructions;
};

Input parseInput(std::string_view input);

void executeInstruction(Field& f, Instruction const& i);

std::string getOutput(Field const& f);

std::string answer1(Input const& in);

void executeInstruction9001(Field& f, Instruction const& i);

std::string answer2(Input const& in);

#endif
