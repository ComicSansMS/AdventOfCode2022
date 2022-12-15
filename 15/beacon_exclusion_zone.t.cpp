#include <beacon_exclusion_zone.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Beacon Exclusion Zone")
{
    SECTION("Point to String")
    {
        std::stringstream sstr;
        sstr << Point{ .x = 42, .y = -23 };
        CHECK(sstr.str() == "(42, -23)");
    }

    char const sample_input[] =
        "Sensor at x=2, y=18: closest beacon is at x=-2, y=15"  "\n"
        "Sensor at x=9, y=16: closest beacon is at x=10, y=16"  "\n"
        "Sensor at x=13, y=2: closest beacon is at x=15, y=3"   "\n"
        "Sensor at x=12, y=14: closest beacon is at x=10, y=16" "\n"
        "Sensor at x=10, y=20: closest beacon is at x=10, y=16" "\n"
        "Sensor at x=14, y=17: closest beacon is at x=10, y=16" "\n"
        "Sensor at x=8, y=7: closest beacon is at x=2, y=10"    "\n"
        "Sensor at x=2, y=0: closest beacon is at x=2, y=10"    "\n"
        "Sensor at x=0, y=11: closest beacon is at x=2, y=10"   "\n"
        "Sensor at x=20, y=14: closest beacon is at x=25, y=17" "\n"
        "Sensor at x=17, y=20: closest beacon is at x=21, y=22" "\n"
        "Sensor at x=16, y=7: closest beacon is at x=15, y=3"   "\n"
        "Sensor at x=14, y=3: closest beacon is at x=15, y=3"   "\n"
        "Sensor at x=20, y=1: closest beacon is at x=15, y=3"   "\n";

    auto const sensor_beacons = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(sensor_beacons == std::vector<SensorBeaconPair> {
            SensorBeaconPair{ .sensor = Point{ 2, 18 }, .beacon = Point{ -2, 15 } },
            SensorBeaconPair{ .sensor = Point{ 9, 16 }, .beacon = Point{ 10, 16 } },
            SensorBeaconPair{ .sensor = Point{ 13, 2 }, .beacon = Point{ 15, 3 } },
            SensorBeaconPair{ .sensor = Point{ 12, 14 }, .beacon = Point{ 10, 16 } },
            SensorBeaconPair{ .sensor = Point{ 10, 20 }, .beacon = Point{ 10, 16 } },
            SensorBeaconPair{ .sensor = Point{ 14, 17 }, .beacon = Point{ 10, 16 } },
            SensorBeaconPair{ .sensor = Point{ 8, 7 }, .beacon = Point{ 2, 10 } },
            SensorBeaconPair{ .sensor = Point{ 2, 0 }, .beacon = Point{ 2, 10 } },
            SensorBeaconPair{ .sensor = Point{ 0, 11 }, .beacon = Point{ 2, 10 } },
            SensorBeaconPair{ .sensor = Point{ 20, 14 }, .beacon = Point{ 25, 17 } },
            SensorBeaconPair{ .sensor = Point{ 17, 20 }, .beacon = Point{ 21, 22 } },
            SensorBeaconPair{ .sensor = Point{ 16, 7 }, .beacon = Point{ 15, 3 } },
            SensorBeaconPair{ .sensor = Point{ 14, 3 }, .beacon = Point{ 15, 3 } },
            SensorBeaconPair{ .sensor = Point{ 20, 1 }, .beacon = Point{ 15, 3 } },
        });
    }

    SECTION("Bounding Circle")
    {
        CHECK(computeBoundingCircle(sensor_beacons[6]) == Circle{ .center = sensor_beacons[6].sensor, .radius = 9 });
    }

    auto const circles = computeBoundingCircles(sensor_beacons);

    SECTION("Row Covered At")
    {
        REQUIRE(!rowCoveredAt(circles[6], -3).has_value());
        REQUIRE(rowCoveredAt(circles[6], -2).has_value());
        CHECK(rowCoveredAt(circles[6], -2).value() == Range{ 8, 8 });
        CHECK(rowCoveredAt(circles[6], -1).value() == Range{ 7, 9 });
        CHECK(rowCoveredAt(circles[6], 0).value() == Range{ 6, 10 });
        CHECK(rowCoveredAt(circles[6], 6).value() == Range{ 0, 16 });
        CHECK(rowCoveredAt(circles[6], 7).value() == Range{ -1, 17 });
        CHECK(rowCoveredAt(circles[6], 8).value() == Range{ 0, 16 });
        CHECK(rowCoveredAt(circles[6], 15).value() == Range{ 7, 9 });
        CHECK(rowCoveredAt(circles[6], 16).value() == Range{ 8, 8 });
        CHECK(!rowCoveredAt(circles[6], 17).has_value());
        CHECK(!rowCoveredAt(circles[6], 18).has_value());
    }

    SECTION("Ranges for Row")
    {
        auto const rr = allRangesForRow(circles, 10);
        REQUIRE(rr.size() == 1);
        CHECK(rr[0] == Range{ .start = -2, .stop = 24 });
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(sensor_beacons, 10) == 26);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(sensor_beacons, Range{ .start = 0, .stop = 20 }) == 56000011);
    }
}
