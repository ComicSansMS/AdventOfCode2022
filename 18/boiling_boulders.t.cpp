#include <boiling_boulders.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Boiling Boulders")
{
    char const sample_input[] =
        "2,2,2" "\n"
        "1,2,2" "\n"
        "3,2,2" "\n"
        "2,1,2" "\n"
        "2,3,2" "\n"
        "2,2,1" "\n"
        "2,2,3" "\n"
        "2,2,4" "\n"
        "2,2,6" "\n"
        "1,2,5" "\n"
        "3,2,5" "\n"
        "2,1,5" "\n"
        "2,3,5" "\n";

    auto const points = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(points == std::vector<Point>{
            Point{ .x = 2, .y = 2, .z = 2 },
            Point{ .x = 1, .y = 2, .z = 2 },
            Point{ .x = 3, .y = 2, .z = 2 },
            Point{ .x = 2, .y = 1, .z = 2 },
            Point{ .x = 2, .y = 3, .z = 2 },
            Point{ .x = 2, .y = 2, .z = 1 },
            Point{ .x = 2, .y = 2, .z = 3 },
            Point{ .x = 2, .y = 2, .z = 4 },
            Point{ .x = 2, .y = 2, .z = 6 },
            Point{ .x = 1, .y = 2, .z = 5 },
            Point{ .x = 3, .y = 2, .z = 5 },
            Point{ .x = 2, .y = 1, .z = 5 },
            Point{ .x = 2, .y = 3, .z = 5 },
        });
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(points) == 64);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(points) == 58);
    }
}
