#ifndef ADVENT_OF_CODE_15_BEACON_EXCLUSION_ZONE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_15_BEACON_EXCLUSION_ZONE_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <experimental/mdspan>

#include <cstdint>
#include <ostream>
#include <optional>
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

struct SensorBeaconPair {
    Point sensor;
    Point beacon;

    friend bool operator==(SensorBeaconPair const&, SensorBeaconPair const&) = default;
};

std::vector<SensorBeaconPair> parseInput(std::string_view input);

int manhattanDistance(Point const& p1, Point const& p2);

struct Circle {
    Point center;
    int radius;

    friend bool operator==(Circle const&, Circle const&) = default;
};

Circle computeBoundingCircle(SensorBeaconPair const& sb);

std::vector<Circle> computeBoundingCircles(std::vector<SensorBeaconPair> const& sbs);

struct Range {
    int start;
    int stop;

    friend bool operator==(Range, Range) = default;
};

inline std::ostream& operator<<(std::ostream& os, Range const& r) {
    return os << fmt::format("{}, {}", r.start, r.stop);
}

std::optional<Range> rowCoveredAt(Circle const& c, int y);

bool rangesOverlap(Range const& r1, Range const& r2);
Range mergeRanges(Range const& r1, Range const& r2);

std::vector<Range> allRangesForRow(std::vector<Circle> const& c, int y);

int answer1(std::vector<SensorBeaconPair> const& sbs, int row);

std::int64_t answer2(std::vector<SensorBeaconPair> const& sbs, Range const& scan_range);

#endif
