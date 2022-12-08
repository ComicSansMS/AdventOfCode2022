#ifndef ADVENT_OF_CODE_08_TREETOP_TREE_HOUSE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_08_TREETOP_TREE_HOUSE_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <cstdint>
#include <string_view>
#include <vector>

using FieldSpanLayout = std::experimental::layout_left;
using FieldSpanExtents = std::experimental::extents<int, std::experimental::dynamic_extent,
                                                         std::experimental::dynamic_extent>;
struct Field {
    std::vector<int> data;
    std::experimental::mdspan<int, FieldSpanExtents, FieldSpanLayout> span;
};

Field parseInput(std::string_view input);

bool visibleFromLeft(Field const& f, int x, int y);

bool visibleFromTop(Field const& f, int x, int y);

bool visibleFromRight(Field const& f, int x, int y);

bool visibleFromBottom(Field const& f, int x, int y);

bool isVisible(Field const& f, int x, int y);

int answer1(Field const& f);

int countLeft(Field const& f, int x, int y);

int countUp(Field const& f, int x, int y);

int countRight(Field const& f, int x, int y);

int countDown(Field const& f, int x, int y);

int scenicScore(Field const& f, int x, int y);

int answer2(Field const& f);

#endif
