#include <blizzard_basin.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Blizzard Basin")
{
    SECTION("Point printing")
    {
        std::stringstream sstr;
        sstr << Point{ 42, 23, -17 };
        CHECK(sstr.str() == "(42, 23, -17)");
    }

    char const sample_input[] =
        "#.#####" "\n"
        "#.....#" "\n"
        "#>....#" "\n"
        "#.....#" "\n"
        "#...v.#" "\n"
        "#.....#" "\n"
        "#####.#" "\n";

    char const sample_input2[] =
        "#.######" "\n"
        "#>>.<^<#" "\n"
        "#.<..<<#" "\n"
        "#>v.><>#" "\n"
        "#<^v^^>#" "\n"
        "######.#" "\n";

    auto const field = parseInput(sample_input);
    auto const field2 = parseInput(sample_input2);

    SECTION("Parse Input")
    {
        CHECK(field.width == 5);
        CHECK(field.height == 5);
        CHECK(fmt::format("{}", field) ==
            "....." "\n"
            ">...." "\n"
            "....." "\n"
            "...v." "\n"
            "....." "\n"
        );
    }

    SECTION("Advance Time")
    {
        auto f = field;
        f = advanceTime(f);
        CHECK(((f.width == 5) && (f.height == 5)));
        CHECK(fmt::format("{}", f) ==
            "....." "\n"
            ".>..." "\n"
            "....." "\n"
            "....." "\n"
            "...v." "\n"
        );
        f = advanceTime(f);
        CHECK(((f.width == 5) && (f.height == 5)));
        CHECK(fmt::format("{}", f) ==
            "...v." "\n"
            "..>.." "\n"
            "....." "\n"
            "....." "\n"
            "....." "\n"
        );
        f = advanceTime(f);
        CHECK(((f.width == 5) && (f.height == 5)));
        CHECK(fmt::format("{}", f) ==
            "....." "\n"
            "...2." "\n"
            "....." "\n"
            "....." "\n"
            "....." "\n"
        );
        f = advanceTime(f);
        CHECK(((f.width == 5) && (f.height == 5)));
        CHECK(fmt::format("{}", f) ==
            "....." "\n"
            "....>" "\n"
            "...v." "\n"
            "....." "\n"
            "....." "\n"
        );
        f = advanceTime(f);
        CHECK(((f.width == 5) && (f.height == 5)));
        CHECK(fmt::format("{}", f) ==
            "....." "\n"
            ">...." "\n"
            "....." "\n"
            "...v." "\n"
            "....." "\n"
        );
    }

    SECTION("Advance Time 2")
    {
        auto f = field2;
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ">>.<^<" "\n"
            ".<..<<" "\n"
            ">v.><>" "\n"
            "<^v^^>" "\n"
        );
        // Minute 1
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ".>3.<." "\n"
            "<..<<." "\n"
            ">2.22." "\n"
            ">v..^<" "\n"
        );
        // Minute 2
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ".2>2.." "\n"
            ".^22^<" "\n"
            ".>2.^>" "\n"
            ".>..<." "\n"
        );
        // Minute 3
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            "<^<22." "\n"
            ".2<.2." "\n"
            "><2>.." "\n"
            "..><.." "\n"
        );
        // Minute 4
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ".<..22" "\n"
            "<<.<.." "\n"
            "<2.>>." "\n"
            ".^22^." "\n"
        );
        // Minute 5
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            "2.v.<>" "\n"
            "<.<..<" "\n"
            ".^>^22" "\n"
            ".2..2." "\n"
        );
        // Minute 6
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ">2.<.<" "\n"
            ".2v^2<" "\n"
            ">..>2>" "\n"
            "<....>" "\n"
        );
        // Minute 7
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ".22^2." "\n"
            "<v.<2." "\n"
            ">>v<>." "\n"
            ">....<" "\n"
        );
        // Minute 8
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ".<>2^." "\n"
            "..<<.<" "\n"
            ".22..>" "\n"
            ".2v^2." "\n"
        );
        // Minute 9
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            "<.2>>." "\n"
            ".<<.<." "\n"
            ">2>2^." "\n"
            ".v><^." "\n"
        );
        // Minute 18
        for (int i = 9; i < 18; ++i) {
            f = advanceTime(f);
        }
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ">2.<.<" "\n"
            ".2v^2<" "\n"
            ">..>2>" "\n"
            "<....>" "\n"
        );
        // wrap around
        for (int i = 18, i_end = f.width * f.height; i < i_end - 1; ++i) {
            f = advanceTime(f);
            CHECK(((f.width == 6) && (f.height == 4)));
            CHECK(fmt::format("{}", f) !=
                ">>.<^<" "\n"
                ".<..<<" "\n"
                ">v.><>" "\n"
                "<^v^^>" "\n"
            );
        }
        f = advanceTime(f);
        CHECK(((f.width == 6) && (f.height == 4)));
        CHECK(fmt::format("{}", f) ==
            ">>.<^<" "\n"
            ".<..<<" "\n"
            ">v.><>" "\n"
            "<^v^^>" "\n"
        );
    }

    auto const cube = buildFieldCube(field2);
    SECTION("Build Field Cube")
    {
        CHECK(cube.width == 6);
        CHECK(cube.height == 4);
        CHECK(cube.depth == 24);
    }
    
    SECTION("Solve")
    {
        CHECK(solve(cube, Point{ .x = 0, .y = 0, .z = 1 }, Point{ .x = cube.width - 1, .y = cube.height - 1, .z = -1 }) == 18);
    }

    SECTION("Answer1")
    {
        CHECK(answer1(cube) == 18);
    }

    SECTION("Answer2")
    {
        CHECK(answer2(cube) == 54);
    }
}
