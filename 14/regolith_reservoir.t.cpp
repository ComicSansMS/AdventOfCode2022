#include <regolith_reservoir.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Distress Signal")
{
    SECTION("Point to String")
    {
        std::stringstream sstr;
        sstr << Point{ .x = 42, .y = -23 };
        CHECK(sstr.str() == "(42, -23)");
    }

    char const sample_input[] =
        "498,4 -> 498,6 -> 496,6"          "\n"
        "503,4 -> 502,4 -> 502,9 -> 494,9" "\n";

    auto const lines = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(lines.size() == 2);
        REQUIRE(lines[0].segments.size() == 2);
        CHECK(lines[0].segments[0].start == Point{ 498, 4 });
        CHECK(lines[0].segments[0].end == Point{ 498, 6 });
        CHECK(lines[0].segments[1].start == Point{ 498, 6 });
        CHECK(lines[0].segments[1].end == Point{ 496, 6 });
        REQUIRE(lines[1].segments.size() == 3);
        CHECK(lines[1].segments[0].start == Point{ 503, 4 });
        CHECK(lines[1].segments[0].end == Point{ 502, 4 });
        CHECK(lines[1].segments[1].start == Point{ 502, 4 });
        CHECK(lines[1].segments[1].end == Point{ 502, 9 });
        CHECK(lines[1].segments[2].start == Point{ 502, 9 });
        CHECK(lines[1].segments[2].end == Point{ 494, 9 });
    }

    auto field = buildField(lines);

    SECTION("Build Field")
    {
        CHECK(field.bound_min == Point{ 494, 0 });
        CHECK(field.bound_max == Point{ 503, 9 });

        CHECK(fmt::format("{}", field) == 
            "......+..." "\n"
            ".........." "\n"
            ".........." "\n"
            ".........." "\n"
            "....#...##" "\n"
            "....#...#." "\n"
            "..###...#." "\n"
            "........#." "\n"
            "........#." "\n"
            "#########." "\n"
        );
    }

    SECTION("Drop Sand")
    {
        CHECK(dropSand(field));
        CHECK(fmt::format("{}", field) ==
            "......+..." "\n"
            ".........." "\n"
            ".........." "\n"
            ".........." "\n"
            "....#...##" "\n"
            "....#...#." "\n"
            "..###...#." "\n"
            "........#." "\n"
            "......o.#." "\n"
            "#########." "\n"
        );

        CHECK(dropSand(field));
        CHECK(fmt::format("{}", field) ==
            "......+..." "\n"
            ".........." "\n"
            ".........." "\n"
            ".........." "\n"
            "....#...##" "\n"
            "....#...#." "\n"
            "..###...#." "\n"
            "........#." "\n"
            ".....oo.#." "\n"
            "#########." "\n"
        );
        for (int i = 0; i < 3; ++i) { CHECK(dropSand(field)); }
        CHECK(fmt::format("{}", field) ==
            "......+..." "\n"
            ".........." "\n"
            ".........." "\n"
            ".........." "\n"
            "....#...##" "\n"
            "....#...#." "\n"
            "..###...#." "\n"
            "......o.#." "\n"
            "....oooo#." "\n"
            "#########." "\n"
        );
        for (int i = 0; i < 17; ++i) { CHECK(dropSand(field)); }
        CHECK(fmt::format("{}", field) ==
            "......+..." "\n"
            ".........." "\n"
            "......o..." "\n"
            ".....ooo.." "\n"
            "....#ooo##" "\n"
            "....#ooo#." "\n"
            "..###ooo#." "\n"
            "....oooo#." "\n"
            "...ooooo#." "\n"
            "#########." "\n"
        );
        for (int i = 0; i < 2; ++i) { CHECK(dropSand(field)); }
        CHECK(fmt::format("{}", field) ==
            "......+..." "\n"
            ".........." "\n"
            "......o..." "\n"
            ".....ooo.." "\n"
            "....#ooo##" "\n"
            "...o#ooo#." "\n"
            "..###ooo#." "\n"
            "....oooo#." "\n"
            ".o.ooooo#." "\n"
            "#########." "\n"
        );
        CHECK(!dropSand(field));
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(field) == 24);
    }

    SECTION("Answer 1")
    {
        CHECK(answer2(lines) == 93);
    }
}
