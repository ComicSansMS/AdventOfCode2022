#include <monkey_map.hpp>

#include <catch.hpp>

TEST_CASE("Monkey Math")
{
    char const sample_input[] =
        "        ...#"     "\n"
        "        .#.."     "\n"
        "        #..."     "\n"
        "        ...."     "\n"
        "...#.......#"     "\n"
        "........#..."     "\n"
        "..#....#...."     "\n"
        "..........#."     "\n"
        "        ...#...." "\n"
        "        .....#.." "\n"
        "        .#......" "\n"
        "        ......#." "\n"
        "\n"
        "10R5L5R10L4R5L5"  "\n";

    auto const input = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(input.map.width == 18);
        CHECK(input.map.height == 14);
        CHECK(fmt::format("{}", input.map) ==
            "                  " "\n"
            "         ...#     " "\n"
            "         .#..     " "\n"
            "         #...     " "\n"
            "         ....     " "\n"
            " ...#.......#     " "\n"
            " ........#...     " "\n"
            " ..#....#....     " "\n"
            " ..........#.     " "\n"
            "         ...#.... " "\n"
            "         .....#.. " "\n"
            "         .#...... " "\n"
            "         ......#. " "\n"
            "                  " "\n");
        REQUIRE(static_cast<int>(input.map.row_width.size()) == input.map.height);
        REQUIRE(static_cast<int>(input.map.col_height.size()) == input.map.width);
        CHECK(input.map.row_width[0] == Range{ .first = -1, .last = -1 });
        for (int i = 1; i <= 4; ++i) { CHECK(input.map.row_width[i] == Range{ .first = 9, .last = 12 }); }
        for (int i = 5; i <= 8; ++i) { CHECK(input.map.row_width[i] == Range{ .first = 1, .last = 12 }); }
        for (int i = 9; i <= 12; ++i) { CHECK(input.map.row_width[i] == Range{ .first = 9, .last = 16 }); }
        CHECK(input.map.row_width[13] == Range{ .first = -1, .last = -1 });
        CHECK(input.map.col_height[0] == Range{ .first = -1, .last = -1 });
        for (int i = 1; i <= 8; ++i) { CHECK(input.map.col_height[i] == Range{ .first = 5, .last = 8 }); }
        for (int i = 9; i <= 12; ++i) { CHECK(input.map.col_height[i] == Range{ .first = 1, .last = 12 }); }
        for (int i = 13; i <= 16; ++i) { CHECK(input.map.col_height[i] == Range{ .first = 9, .last = 12 }); }
        CHECK(input.map.col_height[17] == Range{ .first = -1, .last = -1 });

        CHECK(input.path == std::vector<Step>{
            Step{ Forward{.amount = 10 } },
            Step{ Turn::Right },
            Step{ Forward{.amount = 5 } },
            Step{ Turn::Left },
            Step{ Forward{.amount = 5 } },
            Step{ Turn::Right },
            Step{ Forward{.amount = 10 } },
            Step{ Turn::Left },
            Step{ Forward{.amount = 4 } },
            Step{ Turn::Right },
            Step{ Forward{.amount = 5 } },
            Step{ Turn::Left },
            Step{ Forward{.amount = 5 } },
        });
    }

    SECTION("Determine starting position")
    {
        CHECK(determineStartingPosition(input.map) == Position{ .x = 9, .y = 1, .dir = Direction::Right });
    }

    SECTION("Turn Left")
    {
        CHECK(turnLeft(Direction::Up) == Direction::Left);
        CHECK(turnLeft(Direction::Left) == Direction::Down);
        CHECK(turnLeft(Direction::Down) == Direction::Right);
        CHECK(turnLeft(Direction::Right) == Direction::Up);
    }

    SECTION("Turn Right")
    {
        CHECK(turnRight(Direction::Up) == Direction::Right);
        CHECK(turnRight(Direction::Right) == Direction::Down);
        CHECK(turnRight(Direction::Down) == Direction::Left);
        CHECK(turnRight(Direction::Left) == Direction::Up);
    }

    SECTION("Walk")
    {
        auto pos = walk(determineStartingPosition(input.map), input.map, input.path[0]);
        CHECK(pos == Position{ .x = 11, .y = 1, .dir = Direction::Right });
        pos = walk(pos, input.map, input.path[1]);
        CHECK(pos == Position{ .x = 11, .y = 1, .dir = Direction::Down });
        pos = walk(pos, input.map, input.path[2]);
        CHECK(pos == Position{ .x = 11, .y = 6, .dir = Direction::Down });
        pos = walk(pos, input.map, input.path[3]);
        CHECK(pos == Position{ .x = 11, .y = 6, .dir = Direction::Right });
        pos = walk(pos, input.map, input.path[4]);
        CHECK(pos == Position{ .x = 4, .y = 6, .dir = Direction::Right });
        pos = walk(pos, input.map, input.path[5]);
        CHECK(pos == Position{ .x = 4, .y = 6, .dir = Direction::Down });
        pos = walk(pos, input.map, input.path[6]);
        CHECK(pos == Position{ .x = 4, .y = 8, .dir = Direction::Down });
        pos = walk(pos, input.map, input.path[7]);
        CHECK(pos == Position{ .x = 4, .y = 8, .dir = Direction::Right });
        pos = walk(pos, input.map, input.path[8]);
        CHECK(pos == Position{ .x = 8, .y = 8, .dir = Direction::Right });
        pos = walk(pos, input.map, input.path[9]);
        CHECK(pos == Position{ .x = 8, .y = 8, .dir = Direction::Down });
        pos = walk(pos, input.map, input.path[10]);
        CHECK(pos == Position{ .x = 8, .y = 6, .dir = Direction::Down });
        pos = walk(pos, input.map, input.path[11]);
        CHECK(pos == Position{ .x = 8, .y = 6, .dir = Direction::Right });

        pos = walk(pos, input.map, Step{ Turn::Left });
        CHECK(pos == Position{ .x = 8, .y = 6, .dir = Direction::Up });
        pos = walk(pos, input.map, Step{ Forward{.amount = 100 } });
        CHECK(pos == Position{ .x = 8, .y = 8, .dir = Direction::Up });
        pos = walk(pos, input.map, Step{ Turn::Left });
        CHECK(pos == Position{ .x = 8, .y = 8, .dir = Direction::Left });
        pos = walk(pos, input.map, Step{ Forward{.amount = 100 } });
        CHECK(pos == Position{ .x = 12, .y = 8, .dir = Direction::Left });

        pos = walk(Position{ .x = 12, .y = 10, .dir = Direction::Down }, input.map, Step{ Forward{.amount = 100 } });
        CHECK(pos == Position{ .x = 12, .y = 12, .dir = Direction::Down });
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(input) == 6032);
    }
}
