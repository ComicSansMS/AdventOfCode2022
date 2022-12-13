#include <hill_climbing_algorithm.hpp>

#include <catch.hpp>

#include <sstream>


TEST_CASE("Hill Climbing Algorithm")
{
    SECTION("Point printing")
    {
        std::stringstream sstr;
        sstr << Point{ 42, 23 };
        CHECK(sstr.str() == "(42, 23)");
    }

    char const sample_input[] =
        "Sabqponm" "\n"
        "abcryxxl" "\n"
        "accszExk" "\n"
        "acctuvwj" "\n"
        "abdefghi" "\n";

    auto const map = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(map.data.size() == 40);
        REQUIRE(map.map.extent(0) == 8);
        REQUIRE(map.map.extent(1) == 5);

        CHECK(map.start == Point{ .x = 0, .y = 0 });
        CHECK(map.end == Point{ .x = 5, .y = 2 });

        CHECK(map.map(0, 0) == 0);
        CHECK(map.map(0, 1) == 0);
        CHECK(map.map(1, 0) == 0);
        CHECK(map.map(1, 1) == 1);
        CHECK(map.map(2, 0) == 1);
        CHECK(map.map(0, 2) == 0);
        CHECK(map.map(3, 2) == 18);
        CHECK(map.map(map.end.x, map.end.y) == 25);
    }

    SECTION("All Paths")
    {
        auto const paths = allPathsFromStart(map);
        CHECK(paths.distances(map.end.x, map.end.y) == 31);
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(map) == 31);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(map) == 29);
    }
}
