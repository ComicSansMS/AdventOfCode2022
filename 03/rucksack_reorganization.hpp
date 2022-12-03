#ifndef ADVENT_OF_CODE_03_RUCKSACK_REORGANIZATION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_03_RUCKSACK_REORGANIZATION_HPP_INCLUDE_GUARD

#include <array>
#include <cstdint>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

using Compartment = std::array<int, 53>;

using Rucksack = std::pair<Compartment, Compartment>;

int prio(char c);

std::vector<Rucksack> parseInput(std::string_view input);

int commonItem(Rucksack const& r);

int answer1(std::vector<Rucksack> const& rucksacks);

int commonGroupItem(Rucksack const& r1, Rucksack const& r2, Rucksack const& r3);

int answer2(std::vector<Rucksack> const& rucksacks);

#endif
