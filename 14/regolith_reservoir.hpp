#ifndef ADVENT_OF_CODE_14_REGOLITH_RESERVOIR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_REGOLITH_RESERVOIR_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <experimental/mdspan>

#include <cassert>
#include <cstdint>
#include <ostream>
#include <string_view>
#include <unordered_set>
#include <vector>

struct Point {
    int x;                                                  // LCOV_EXCL_LINE
    int y;                                                  // LCOV_EXCL_LINE

    friend bool operator==(Point, Point) = default;         // LCOV_EXCL_LINE
};

template<>
struct fmt::formatter<Point>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Point const& p, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

inline std::ostream& operator<<(std::ostream& os, Point const& p) {
    return os << fmt::format("{}", p);
}

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct HashPoint {
    inline std::size_t operator()(Point const& v) const {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        return seed;
    }
};

struct LineSegment {
    Point start;
    Point end;
};

struct Line {
    std::vector<LineSegment> segments;
};

std::vector<Line> parseInput(std::string_view input);

enum class Cell {
    Empty,
    Rock,
    Sand
};

struct FieldSpanLayout {
    template<class Extents>
    struct mapping {
        using extents_type = Extents;
        using rank_type = typename Extents::rank_type;
        using size_type = typename Extents::size_type;
        using layout_type = FieldSpanLayout;

        mapping(Point const& start, int width, int height) noexcept
            : extents_(width, height), start_(start)
        {
            assert(width > 0);
            assert(height > 0);
        }

        constexpr const extents_type& extents() const {
            return extents_;
        }

        constexpr size_type
        required_span_size() const noexcept {
            return extents_.extent(0) * extents_.extent(1);
        }

        template<class RowIndex, class ColIndex>
        constexpr size_type
        operator()(RowIndex row, ColIndex col) const noexcept {
            assert((row >= start_.x) && (row < start_.x + extents_.extent(0)));     // LCOV_EXCL_LINE
            assert((col >= start_.y) && (col < start_.y + extents_.extent(1)));     // LCOV_EXCL_LINE
            return (col - start_.y) * extents_.extent(0) + (row - start_.x);
        }

        static constexpr bool is_always_unique() noexcept { return true; }
        static constexpr bool is_always_contiguous() noexcept { return true; }
        static constexpr bool is_always_strided() noexcept { return true; }

        static constexpr bool is_unique() noexcept { return true; }
        static constexpr bool is_contiguous() noexcept { return true; }
        static constexpr bool is_strided() noexcept { return true; }

        Extents extents_;
        Point start_;
    };
};

using FieldSpan = std::experimental::mdspan<Cell, std::experimental::dextents<int, 2>, FieldSpanLayout>;

struct Field {
    Point bound_min;
    Point bound_max;
    int width;
    int height;
    std::vector<Cell> data;
    FieldSpan field;

    Field(Point const& bmin, Point const& bmax);
    Field(Field const&) = delete;
    Field& operator=(Field const&) = delete;
    Field(Field&&) = default;
    Field& operator=(Field&&) = default;
    
};

Field buildField(std::vector<Line> const& lines);

template<>
struct fmt::formatter<Field>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Field const& f, FormatContext& ctx) {
        for (int iy = f.bound_min.y; iy <= f.bound_max.y; ++iy) {
            for (int ix = f.bound_min.x; ix <= f.bound_max.x; ++ix) {
                if ((ix == 500) && (iy == 0)) {
                    fmt::format_to(ctx.out(), "+");
                } else {
                    Cell const c = f.field(ix, iy);
                    if (c == Cell::Empty) {
                        fmt::format_to(ctx.out(), ".");
                    } else if (c == Cell::Rock) {
                        fmt::format_to(ctx.out(), "#");
                    } else {
                        assert(c == Cell::Sand);
                        fmt::format_to(ctx.out(), "o");
                    }
                }
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

bool dropSand(Field& f);

int answer1(Field& f);

int answer2(std::vector<Line> const& lines);

#endif
