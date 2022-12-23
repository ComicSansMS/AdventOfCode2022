#include <unstable_diffusion.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Monkey Math")
{
    SECTION("Point printing")
    {
        std::stringstream sstr;
        sstr << Point{ 42, 23 };
        CHECK(sstr.str() == "(42, 23)");
    }

    char const sample_input[] =
        "....#.." "\n"
        "..###.#" "\n"
        "#...#.#" "\n"
        ".#...##" "\n"
        "#.###.." "\n"
        "##.#.##" "\n"
        ".#..#.." "\n";

    auto const field = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(field.points.size() == 22);
        CHECK(field.min == Point{ .x = 0, .y = 0 });
        CHECK(field.max == Point{ .x = 7, .y = 7 });
        CHECK(!field.points.contains(Point{ .x = 0, .y = 0 }));
        CHECK(!field.points.contains(Point{ .x = 1, .y = 0 }));
        CHECK(!field.points.contains(Point{ .x = 2, .y = 0 }));
        CHECK(!field.points.contains(Point{ .x = 3, .y = 0 }));
        CHECK(!field.points.contains(Point{ .x = 5, .y = 0 }));
        CHECK(!field.points.contains(Point{ .x = 6, .y = 0 }));
        CHECK(field.points.contains(Point{ .x = 4, .y = 0 }));
        CHECK(field.points.contains(Point{ .x = 2, .y = 1 }));
        CHECK(field.points.contains(Point{ .x = 3, .y = 1 }));
        CHECK(field.points.contains(Point{ .x = 4, .y = 1 }));
        CHECK(field.points.contains(Point{ .x = 6, .y = 1 }));
        CHECK(field.points.contains(Point{ .x = 0, .y = 2 }));
        CHECK(field.points.contains(Point{ .x = 4, .y = 2 }));
        CHECK(field.points.contains(Point{ .x = 6, .y = 2 }));
        CHECK(field.points.contains(Point{ .x = 1, .y = 3 }));
        CHECK(field.points.contains(Point{ .x = 5, .y = 3 }));
        CHECK(field.points.contains(Point{ .x = 6, .y = 3 }));
        CHECK(field.points.contains(Point{ .x = 0, .y = 4 }));
        CHECK(field.points.contains(Point{ .x = 2, .y = 4 }));
        CHECK(field.points.contains(Point{ .x = 3, .y = 4 }));
        CHECK(field.points.contains(Point{ .x = 4, .y = 4 }));
        CHECK(field.points.contains(Point{ .x = 0, .y = 5 }));
        CHECK(field.points.contains(Point{ .x = 1, .y = 5 }));
        CHECK(field.points.contains(Point{ .x = 3, .y = 5 }));
        CHECK(field.points.contains(Point{ .x = 5, .y = 5 }));
        CHECK(field.points.contains(Point{ .x = 6, .y = 5 }));
        CHECK(field.points.contains(Point{ .x = 1, .y = 6 }));
        CHECK(field.points.contains(Point{ .x = 4, .y = 6 }));
    }

    char const sample_input_small[] =
        "....." "\n"
        "..##." "\n"
        "..#.." "\n"
        "....." "\n"
        "..##." "\n"
        "....." "\n";

    auto const small_field = parseInput(sample_input_small);

    SECTION("Move")
    {
        Field f = small_field;
        CHECK(fmt::format("{}", f) == 
            "....." "\n"
            "..##." "\n"
            "..#.." "\n"
            "....." "\n"
            "..##." "\n"
            "....." "\n");
        CHECK(move(f, 0).did_change);
        f.points = move(f, 0).points;
        CHECK(fmt::format("{}", f) ==
            "..##." "\n"
            "....." "\n"
            "..#.." "\n"
            "...#." "\n"
            "..#.." "\n"
            "....." "\n");
        CHECK(move(f, 1).did_change);
        f.points = move(f, 1).points;
        CHECK(fmt::format("{}", f) ==
            "....." "\n"
            "..##." "\n"
            ".#..." "\n"
            "....#" "\n"
            "....." "\n"
            "..#.." "\n");
        CHECK(move(f, 2).did_change);
        f.points = move(f, 2).points;
        CHECK(fmt::format("{}", f) ==
            "..#.." "\n"
            "....#" "\n"
            "#...." "\n"
            "....#" "\n"
            "....." "\n"
            "..#.." "\n");
        auto const new_f = move(f, 3);
        CHECK(new_f.points == f.points);
        CHECK(!new_f.did_change);
    }

    SECTION("Move larger")
    {
        Field f = field;
        f.min = Point{ .x = -3, .y = -2 };
        f.max = Point{ .x = 11, .y = 10 };
        CHECK(fmt::format("{}", f) ==
            ".............." "\n"
            ".............." "\n"
            ".......#......" "\n"
            ".....###.#...." "\n"
            "...#...#.#...." "\n"
            "....#...##...." "\n"
            "...#.###......" "\n"
            "...##.#.##...." "\n"
            "....#..#......" "\n"
            ".............." "\n"
            ".............." "\n"
            ".............." "\n");
        f.points = move(f, 0).points;
        CHECK(fmt::format("{}", f) ==
            ".............." "\n"
            ".......#......" "\n"
            ".....#...#...." "\n"
            "...#..#.#....." "\n"
            ".......#..#..." "\n"
            "....#.#.##...." "\n"
            "..#..#.#......" "\n"
            "..#.#.#.##...." "\n"
            ".............." "\n"
            "....#..#......" "\n"
            ".............." "\n"
            ".............." "\n");
        f.points = move(f, 1).points;
        CHECK(fmt::format("{}", f) ==
            ".............." "\n"
            ".......#......" "\n"
            "....#.....#..." "\n"
            "...#..#.#....." "\n"
            ".......#...#.." "\n"
            "...#..#.#....." "\n"
            ".#...#.#.#...." "\n"
            ".............." "\n"
            "..#.#.#.##...." "\n"
            "....#..#......" "\n"
            ".............." "\n"
            ".............." "\n");
        f.points = move(f, 2).points;
        CHECK(fmt::format("{}", f) ==
            ".............." "\n"
            ".......#......" "\n"
            ".....#....#..." "\n"
            "..#..#...#...." "\n"
            ".......#...#.." "\n"
            "...#..#.#....." "\n"
            ".#..#.....#..." "\n"
            ".......##....." "\n"
            "..##.#....#..." "\n"
            "...#.........." "\n"
            ".......#......" "\n"
            ".............." "\n");
        f.points = move(f, 3).points;
        CHECK(fmt::format("{}", f) ==
            ".............." "\n"
            ".......#......" "\n"
            "......#....#.." "\n"
            "..#...##......" "\n"
            "...#.....#.#.." "\n"
            ".........#...." "\n"
            ".#...###..#..." "\n"
            "..#......#...." "\n"
            "....##....#..." "\n"
            "....#........." "\n"
            ".......#......" "\n"
            ".............." "\n");
        f.points = move(f, 4).points;
        CHECK(fmt::format("{}", f) ==
            ".......#......" "\n"
            ".............." "\n"
            "..#..#.....#.." "\n"
            ".........#...." "\n"
            "......##...#.." "\n"
            ".#.#.####....." "\n"
            "...........#.." "\n"
            "....##..#....." "\n"
            "..#..........." "\n"
            "..........#..." "\n"
            "....#..#......" "\n"
            ".............." "\n");
        f.points = move(f, 5).points;
        f.points = move(f, 6).points;
        f.points = move(f, 7).points;
        f.points = move(f, 8).points;
        f = move(f, 9);
        CHECK(f.did_change);
        CHECK(fmt::format("{}", f) ==
            "......#....." "\n"
            "..........#." "\n"
            ".#.#..#....." "\n"
            ".....#......" "\n"
            "..#.....#..#" "\n"
            "#......##..." "\n"
            "....##......" "\n"
            ".#........#." "\n"
            "...#.#..#..." "\n"
            "............" "\n"
            "...#..#..#.." "\n");
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(field) == 110);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(field) == 20);
    }
}
