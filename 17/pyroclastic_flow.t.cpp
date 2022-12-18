#include <pyroclastic_flow.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Pyroclastic Flow")
{
    char const sample_input[] = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>" "\n";

    auto const instructions = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(instructions.size() == 40);
        CHECK(instructions[0] == Instruction::Right);
        CHECK(instructions[1] == Instruction::Right);
        CHECK(instructions[2] == Instruction::Right);
        CHECK(instructions[3] == Instruction::Left);
        CHECK(instructions[4] == Instruction::Left);
        CHECK(instructions[5] == Instruction::Right);
        CHECK(instructions[6] == Instruction::Left);
        CHECK(instructions[7] == Instruction::Right);
    }

    SECTION("Spawn Rock")
    {
        Field f;
        CHECK(fmt::format("{}", f) ==
            "+-------+\n"
        );
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|..####.|\n"
            "+-------+\n"
        );
        CHECK(f.max_y == 1);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|...#...|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 4);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|..#....|" "\n"
            "|..#....|" "\n"
            "|####...|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 6);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|....#..|" "\n"
            "|..#.#..|" "\n"
            "|..#.#..|" "\n"
            "|#####..|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 7);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|....##.|" "\n"
            "|....##.|" "\n"
            "|....#..|" "\n"
            "|..#.#..|" "\n"
            "|..#.#..|" "\n"
            "|#####..|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 9);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|.####..|" "\n"
            "|....##.|" "\n"
            "|....##.|" "\n"
            "|....#..|" "\n"
            "|..#.#..|" "\n"
            "|..#.#..|" "\n"
            "|#####..|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 10);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|..#....|" "\n"
            "|.###...|" "\n"
            "|..#....|" "\n"
            "|.####..|" "\n"
            "|....##.|" "\n"
            "|....##.|" "\n"
            "|....#..|" "\n"
            "|..#.#..|" "\n"
            "|..#.#..|" "\n"
            "|#####..|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 13);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|.....#.|" "\n"
            "|.....#.|" "\n"
            "|..####.|" "\n"
            "|.###...|" "\n"
            "|..#....|" "\n"
            "|.####..|" "\n"
            "|....##.|" "\n"
            "|....##.|" "\n"
            "|....#..|" "\n"
            "|..#.#..|" "\n"
            "|..#.#..|" "\n"
            "|#####..|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 15);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|....#..|" "\n"
            "|....#..|" "\n"
            "|....##.|" "\n"
            "|....##.|" "\n"
            "|..####.|" "\n"
            "|.###...|" "\n"
            "|..#....|" "\n"
            "|.####..|" "\n"
            "|....##.|" "\n"
            "|....##.|" "\n"
            "|....#..|" "\n"
            "|..#.#..|" "\n"
            "|..#.#..|" "\n"
            "|#####..|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
        CHECK(f.max_y == 17);
        spawnRock(f, instructions);
        CHECK(fmt::format("{}", f) ==
            "|....#..|" "\n"
            "|....#..|" "\n"
            "|....##.|" "\n"
            "|##..##.|" "\n"
            "|######.|" "\n"
            "|.###...|" "\n"
            "|..#....|" "\n"
            "|.####..|" "\n"
            "|....##.|" "\n"
            "|....##.|" "\n"
            "|....#..|" "\n"
            "|..#.#..|" "\n"
            "|..#.#..|" "\n"
            "|#####..|" "\n"
            "|..###..|" "\n"
            "|...#...|" "\n"
            "|..####.|" "\n"
            "+-------+" "\n"
        );
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(instructions) == 3068);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(instructions) == 1514285714288);
    }
}
