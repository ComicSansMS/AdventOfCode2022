#ifndef ADVENT_OF_CODE_04_CAMP_CLEANUP_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_04_CAMP_CLEANUP_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

struct Range {
    int64_t first;
    int64_t last;

    friend bool operator==(Range, Range) = default;
};

struct Assignment {
    Range first;
    Range second;
    
    friend bool operator==(Assignment const&, Assignment const&) = default;
};

std::vector<Assignment> parseInput(std::string_view input);

bool fullyContainedRange(Assignment const& a);

int64_t answer1(std::vector<Assignment> const& v);

bool doesOverlap(Assignment const& a);

int64_t answer2(std::vector<Assignment> const& v);

#endif
