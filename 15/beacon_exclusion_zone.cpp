#include <beacon_exclusion_zone.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <regex>


std::vector<SensorBeaconPair> parseInput(std::string_view input)
{
    std::regex rx_line(R"(Sensor at x=([-\d]+), y=([-\d]+): closest beacon is at x=([-\d]+), y=([-\d]+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    auto parseLine = [](std::match_results<std::string_view::iterator> const& match) -> SensorBeaconPair {
        SensorBeaconPair sb;
        sb.sensor.x = std::stoi(match[1]);
        return SensorBeaconPair{ .sensor = Point{.x = std::stoi(match[1]), .y = std::stoi(match[2]) },
                                 .beacon = Point{.x = std::stoi(match[3]), .y = std::stoi(match[4]) } };
    };

    return ranges::make_subrange(it_begin, it_end) | ranges::views::transform(parseLine) | ranges::to<std::vector>;
}

int manhattanDistance(Point const& p1, Point const& p2)
{
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

Circle computeBoundingCircle(SensorBeaconPair const& sb)
{
    return Circle{ .center = sb.sensor, .radius = manhattanDistance(sb.beacon, sb.sensor) };
}

std::vector<Circle> computeBoundingCircles(std::vector<SensorBeaconPair> const& sbs)
{
    return sbs | ranges::views::transform(computeBoundingCircle) | ranges::to<std::vector>;
}

std::optional<Range> rowCoveredAt(Circle const& c, int y)
{
    int const distance = std::abs(y - c.center.y);
    if (distance > c.radius) {
        return std::nullopt;
    }
    int const offset = (c.radius - distance);
    assert(offset >= 0);
    return Range{ .start = c.center.x - offset, .stop = c.center.x + offset };
}

bool rangesOverlap(Range const& r1, Range const& r2)
{
    auto const& [l, r] = std::minmax(r1, r2, [](Range const& r1, Range const& r2) { return r1.start < r2.start; });;
    return (l.stop >= r.start);
}

Range mergeRanges(Range const& r1, Range const& r2)
{
    assert(rangesOverlap(r1, r2));
    return Range{ .start = std::min(r1.start, r2.start), .stop = std::max(r1.stop, r2.stop) };
}

std::vector<Range> allRangesForRow(std::vector<Circle> const& c, int y)
{
    std::vector<Range> ret;
    ret.reserve(c.size());
    for (auto const& cc : c) {
        auto const rc = rowCoveredAt(cc, y);
        if (rc) { ret.push_back(*rc); }
    }

    for (;;) {
        bool retry = false;
        for (int j = 1; j < static_cast<int>(ret.size()); ++j) {
            for (int i = 0; i < j; ++i) {
                if (rangesOverlap(ret[i], ret[j])) {
                    assert(i < j);
                    ret[i] = mergeRanges(ret[i], ret[j]);
                    std::swap(ret[j], ret[ret.size() - 1]);
                    ret.pop_back();
                    retry = true;
                    break;
                }
            }
            if (retry) { break; }
        }
        if (!retry) { break; }
    }

    return ret;
}

int answer1(std::vector<SensorBeaconPair> const& sbs, int row)
{
    auto const circles = computeBoundingCircles(sbs);
    auto const v = allRangesForRow(circles, row);
    assert(v.size() == 1);
    return v[0].stop - v[0].start;
}

std::int64_t answer2(std::vector<SensorBeaconPair> const& sbs, Range const& scan_range)
{
    auto const circles = computeBoundingCircles(sbs);
    for (int iy = scan_range.start; iy <= scan_range.stop; ++iy) {
        auto const rr = allRangesForRow(circles, iy);
        if (rr.size() > 1) {
            int const free_x = rr[0].stop + 1;
            return static_cast<std::int64_t>(free_x) * 4000000 + static_cast<std::int64_t>(iy);
        }
    }
    return -1;                  // LCOV_EXCL_LINE
}
