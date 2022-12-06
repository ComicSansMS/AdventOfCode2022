#ifndef ADVENT_OF_CODE_06_TUNING_TROUBLE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_06_TUNING_TROUBLE_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string>
#include <string_view>

std::string parseInput(std::string_view input);

std::string::const_iterator findMarker(std::string const& str, int window_size);

std::size_t answer1(std::string const& str);

std::size_t answer2(std::string const& str);

#endif
