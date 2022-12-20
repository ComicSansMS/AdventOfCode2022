#include <grove_positioning_system.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>


#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <tuple>

std::list<int64_t> parseInput(std::string_view input)
{
    auto toNumber = [](auto s) -> int64_t { return std::stoi(ranges::to<std::string>(s)); };
    return input | ranges::views::split('\n') | ranges::views::transform(toNumber) | ranges::to<std::list>;
}

std::vector<Tracker> buildTracker(std::list<int64_t>& l)
{
    std::vector<Tracker> trackers;
    trackers.reserve(l.size());

    for (auto it = l.begin(), it_end = l.end(); it != it_end; ++it) {
        trackers.push_back(Tracker{ .i = *it, .it = it });
    }
    return trackers;
}

std::list<int64_t>::iterator cyclicAdvance(std::list<int64_t>& l, std::list<int64_t>::iterator it, int64_t i)
{
    auto const it_begin = l.begin();
    auto const it_end = l.end();
    int64_t const lsize = static_cast<int64_t>(l.size());
    assert(it != it_end);
    auto ret = it;
    int64_t offset = i % (lsize - 1);
    if (offset > 0) {
        for (int64_t j = 0; j <= offset; ++j) {
            if (ret == it_end) { ret = it_begin; }
            ++ret;
        }
        if (ret == it_end) { ret = it_begin; }
    } else if (offset < 0) {
        for (int64_t j = 0; j < -offset; ++j) {
            if (ret == it_begin) { ret = it_end; }
            --ret;
        }
        if (ret == it_begin) { ret = it_end; }
    }

    return ret;
}

inline void moveTo(std::list<int64_t>& l, std::list<int64_t>::iterator it, std::list<int64_t>::iterator to)
{
    l.splice(to, l, it);
}

void decodeStep(std::list<int64_t>& l, Tracker const& t)
{
    if (t.i != 0) {
        auto new_it = cyclicAdvance(l, t.it, t.i);
        if (new_it != t.it) {
            moveTo(l, t.it, new_it);
        }
    }
}

inline void decode(std::list<int64_t>& l, std::vector<Tracker> const& trackers)
{
    for (auto const& t : trackers) {
        decodeStep(l, t);
    }
}

void decode(std::list<int64_t>& l)
{
    std::vector<Tracker> const trackers = buildTracker(l);
    decode(l, trackers);
}

int64_t getElement(std::list<int64_t> const& l, std::size_t at)
{
    assert(at < l.size());
    auto it = l.begin();
    std::advance(it, at);
    return *it;
}

int64_t answer1(std::list<int64_t> l)
{
    decode(l);
    int64_t const index0 = std::distance(l.begin(), std::find(l.begin(), l.end(), 0));
    int64_t const i1 = (index0 + 1000) % static_cast<int64_t>(l.size());
    int64_t const i2 = (index0 + 2000) % static_cast<int64_t>(l.size());
    int64_t const i3 = (index0 + 3000) % static_cast<int64_t>(l.size());
    int64_t const n1 = getElement(l, i1);
    int64_t const n2 = getElement(l, i2);
    int64_t const n3 = getElement(l, i3);
    return n1 + n2 + n3;
}

std::list<int64_t> applyDecryptionKey(std::list<int64_t> l)
{
    int64_t const decryption_key = 811589153;
    for (auto& i : l) { i *= decryption_key; }
    return l;
}

void decode2(std::list<int64_t>& l)
{
    std::vector<Tracker> const trackers = buildTracker(l);
    for (int i = 0; i < 10; ++i) {
        for (auto const& t : trackers) {
            decodeStep(l, t);
        }
    }
}

int64_t answer2(std::list<int64_t> const& numbers)
{
    auto l = applyDecryptionKey(numbers);
    decode2(l);
    int64_t const index0 = std::distance(l.begin(), std::find(l.begin(), l.end(), 0));
    int64_t const i1 = (index0 + 1000) % static_cast<int64_t>(l.size());
    int64_t const i2 = (index0 + 2000) % static_cast<int64_t>(l.size());
    int64_t const i3 = (index0 + 3000) % static_cast<int64_t>(l.size());
    int64_t const n1 = getElement(l, i1);
    int64_t const n2 = getElement(l, i2);
    int64_t const n3 = getElement(l, i3);
    return n1 + n2 + n3;
}

