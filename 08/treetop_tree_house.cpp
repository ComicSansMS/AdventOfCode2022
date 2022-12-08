#include <treetop_tree_house.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/format.h>

#include <cassert>
#include <limits>

Field parseInput(std::string_view input)
{
    std::vector<int> data;
    int field_width = 0;
    int field_height = 0;
    for (auto const line : input | ranges::views::split('\n')) {
        std::vector<int> const line_numbers =
            line
            | ranges::views::transform([](char c) -> int {
                    assert((c >= '0') && (c <= '9'));
                    return c - '0';
                })
            | ranges::to<std::vector>;
        if (field_width == 0) { field_width = static_cast<int>(line_numbers.size()); }
        assert(field_width == static_cast<int>(line_numbers.size()));
        data.insert(ranges::end(data), ranges::begin(line_numbers), ranges::end(line_numbers));
        ++field_height;
    }
    Field ret;
    ret.data = std::move(data);
    ret.span = decltype(ret.span){ ret.data.data(), field_width, field_height };
    return ret;
}

bool visibleFromLeft(Field const& f, int x, int y)
{
    int const t = f.span(x, y);
    for (int ix = x - 1; ix >= 0; --ix) {
        if (f.span(ix, y) >= t) {
            return false;
        }
    }
    return true;
}

bool visibleFromTop(Field const& f, int x, int y)
{
    int const t = f.span(x, y);
    for (int iy = y - 1; iy >= 0; --iy) {
        if (f.span(x, iy) >= t) {
            return false;
        }
    }
    return true;
}

bool visibleFromRight(Field const& f, int x, int y)
{
    int const t = f.span(x, y);
    for (int ix = x + 1; ix < f.span.extent(0); ++ix) {
        if (f.span(ix, y) >= t) {
            return false;
        }
    }
    return true;
}

bool visibleFromBottom(Field const& f, int x, int y)
{
    int const t = f.span(x, y);
    for (int iy = y + 1; iy < f.span.extent(1); ++iy) {
        if (f.span(x, iy) >= t) {
            return false;
        }
    }
    return true;
}

bool isVisible(Field const& f, int x, int y)
{
    return visibleFromLeft(f, x, y) ||
        visibleFromRight(f, x, y) ||
        visibleFromTop(f, x, y) ||
        visibleFromBottom(f, x, y);
}

int answer1(Field const& f)
{
    int count = 0;
    for (int ix = 0; ix < f.span.extent(0); ++ix) {
        for (int iy = 0; iy < f.span.extent(1); ++iy) {
            if (isVisible(f, ix, iy)) { ++count; }
        }
    }
    return count;
}


int countLeft(Field const& f, int x, int y)
{
    int ret = 0;
    int const t = f.span(x, y);
    for (int ix = x - 1; ix >= 0; --ix) {
        ++ret;
        if (f.span(ix, y) >= t) {
            break;
        }
    }
    return ret;
}

int countUp(Field const& f, int x, int y)
{
    int ret = 0;
    int const t = f.span(x, y);
    for (int iy = y - 1; iy >= 0; --iy) {
        ++ret;
        if (f.span(x, iy) >= t) {
            break;
        }
    }
    return ret;
}

int countRight(Field const& f, int x, int y)
{
    int ret = 0;
    int const t = f.span(x, y);
    for (int ix = x + 1; ix < f.span.extent(0); ++ix) {
        ++ret;
        if (f.span(ix, y) >= t) {
            break;
        }
    }
    return ret;
}


int countDown(Field const& f, int x, int y)
{
    int ret = 0;
    int const t = f.span(x, y);
    for (int iy = y + 1; iy < f.span.extent(1); ++iy) {
        ++ret;
        if (f.span(x, iy) >= t) {
            break;
        }
    }
    return ret;
}

int scenicScore(Field const& f, int x, int y)
{
    return countLeft(f, x, y) * countRight(f, x, y) * countUp(f, x, y) * countDown(f, x, y);
}

int answer2(Field const& f)
{
    int max_score = 0;
    for (int ix = 0; ix < f.span.extent(0); ++ix) {
        for (int iy = 0; iy < f.span.extent(1); ++iy) {
            max_score = std::max(max_score, scenicScore(f, ix, iy));
        }
    }
    return max_score;

}
