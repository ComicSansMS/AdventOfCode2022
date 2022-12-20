#ifndef ADVENT_OF_CODE_20_GROVE_POSITIONING_SYSTEM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_20_GROVE_POSITIONING_SYSTEM_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <cstdint>
#include <list>
#include <string_view>
#include <vector>

using std::int64_t;

std::list<int64_t> parseInput(std::string_view input);

struct Tracker {
    int64_t i;
    std::list<int64_t>::iterator it;
};

std::vector<Tracker> buildTracker(std::list<int64_t>& l);

void decodeStep(std::list<int64_t>& l, Tracker const& t);

void decode(std::list<int64_t>& l);

void decode(std::list<int64_t>& l, std::vector<Tracker> const& trackers);

int64_t answer1(std::list<int64_t> l);

std::list<int64_t> applyDecryptionKey(std::list<int64_t> l);

void decode2(std::list<int64_t>& l);

int64_t answer2(std::list<int64_t> const& numbers);

#endif
