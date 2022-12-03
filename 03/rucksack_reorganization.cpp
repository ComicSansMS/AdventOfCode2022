#include <rucksack_reorganization.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/chunk.hpp>

#include <cassert>
#include <optional>
#include <regex>
#include <string>

int prio(char c)
{
    if ((c >= 'A') && (c <= 'Z')) {
        return (c - 'A') + 27;
    } else {
        assert((c >= 'a') && (c <= 'z'));
        return (c - 'a') + 1;
    }
}

std::vector<Rucksack> parseInput(std::string_view input)
{
    std::vector<Rucksack> ret;
    auto it_line_start = ranges::begin(input);
    for (auto it = ranges::begin(input), it_end = ranges::end(input); it != it_end; ++it) {
        if (*it == '\n') {
            ret.emplace_back();
            Rucksack& r = ret.back();
            auto const line_len = it - it_line_start;
            assert(line_len % 2 == 0);
            auto const midpoint = it_line_start + (line_len / 2);
            for (char const c : ranges::subrange(it_line_start, midpoint)) {
                ++r.first[prio(c)];
            }
            for (char const c : ranges::subrange(midpoint, it)) {
                ++r.second[prio(c)];
            }
            it_line_start = (it + 1);
        }
    }
    return ret;
}

int commonItem(Rucksack const& r)
{
    for (int i = 1; i <= 52; ++i) {
        if ((r.first[i] > 0) && (r.second[i] > 0)) { return i; }
    }
    return 0;
}

int answer1(std::vector<Rucksack> const& rucksacks)
{
    return ranges::accumulate(rucksacks, 0, ranges::plus{}, commonItem);
}

int commonGroupItem(Rucksack const& r1, Rucksack const& r2, Rucksack const& r3)
{
    auto const hasItem = [](Rucksack const& r, int item) -> bool {
        return (r.first[item] > 0) || (r.second[item] > 0);
    };
    for (int i = 1; i <= 52; ++i) {
        if (hasItem(r1, i) && hasItem(r2, i) && hasItem(r3, i)) { return i; }
    }
    return 0;
}

int answer2(std::vector<Rucksack> const& rucksacks)
{
    assert((rucksacks.size() % 3) == 0);
    return ranges::accumulate(rucksacks | ranges::views::chunk(3), 0, ranges::plus{},
                              [](auto const& r) -> int { return commonGroupItem(r[0], r[1], r[2]); });
}
