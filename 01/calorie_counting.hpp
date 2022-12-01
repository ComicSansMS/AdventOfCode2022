#ifndef ADVENT_OF_CODE_01_CALORIE_COUNTING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_01_CALORIE_COUNTING_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

struct Elf {
    std::vector<std::int64_t> calories;

    friend bool operator==(Elf const&, Elf const&) = default;
};

std::vector<Elf> parseInput(std::string_view input);

std::int64_t totalCalories(Elf const& e);

std::int64_t maxCalorieElf(std::vector<Elf> const& v);

std::int64_t answer1(std::vector<Elf> const& v);

std::int64_t answer2(std::vector<Elf> const& v);

#endif
