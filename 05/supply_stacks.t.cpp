#include <supply_stacks.hpp>

#include <catch.hpp>

TEST_CASE("Supply Stacks")
{
    char const sample_input[] =
        "    [D]    "        "\n"
        "[N] [C]    "        "\n"
        "[Z] [M] [P]"        "\n"
        " 1   2   3 "        "\n"
        ""                   "\n"
        "move 1 from 2 to 1" "\n"
        "move 3 from 1 to 3" "\n"
        "move 2 from 2 to 1" "\n"
        "move 1 from 1 to 2" "\n";

    auto const input = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(input.field.stacks.size() == 3);
        CHECK(input.field.stacks[0] == std::vector<char>{ 'Z', 'N' });
        CHECK(input.field.stacks[1] == std::vector<char>{ 'M', 'C', 'D' });
        CHECK(input.field.stacks[2] == std::vector<char>{ 'P' });

        REQUIRE(input.instructions.size() == 4);
        CHECK(input.instructions[0] == Instruction{ .amount = 1, .from = 2, .to = 1 });
        CHECK(input.instructions[1] == Instruction{ .amount = 3, .from = 1, .to = 3 });
        CHECK(input.instructions[2] == Instruction{ .amount = 2, .from = 2, .to = 1 });
        CHECK(input.instructions[3] == Instruction{ .amount = 1, .from = 1, .to = 2 });
    }

    SECTION("Execute Instruction")
    {
        Field f = input.field;
        executeInstruction(f, input.instructions[0]);
        CHECK(f.stacks[0] == std::vector<char>{ 'Z', 'N', 'D' });
        CHECK(f.stacks[1] == std::vector<char>{ 'M', 'C' });
        CHECK(f.stacks[2] == std::vector<char>{ 'P' });

        executeInstruction(f, input.instructions[1]);
        CHECK(f.stacks[0] == std::vector<char>{ });
        CHECK(f.stacks[1] == std::vector<char>{ 'M', 'C' });
        CHECK(f.stacks[2] == std::vector<char>{ 'P', 'D', 'N', 'Z' });

        executeInstruction(f, input.instructions[2]);
        CHECK(f.stacks[0] == std::vector<char>{ 'C', 'M' });
        CHECK(f.stacks[1] == std::vector<char>{ });
        CHECK(f.stacks[2] == std::vector<char>{ 'P', 'D', 'N', 'Z' });

        executeInstruction(f, input.instructions[3]);
        CHECK(f.stacks[0] == std::vector<char>{ 'C' });
        CHECK(f.stacks[1] == std::vector<char>{ 'M' });
        CHECK(f.stacks[2] == std::vector<char>{ 'P', 'D', 'N', 'Z' });
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(input) == "CMZ");
    }

    SECTION("Execute Instruction 9001")
    {
        Field f = input.field;
        executeInstruction9001(f, input.instructions[0]);
        CHECK(f.stacks[0] == std::vector<char>{ 'Z', 'N', 'D' });
        CHECK(f.stacks[1] == std::vector<char>{ 'M', 'C' });
        CHECK(f.stacks[2] == std::vector<char>{ 'P' });

        executeInstruction9001(f, input.instructions[1]);
        CHECK(f.stacks[0] == std::vector<char>{ });
        CHECK(f.stacks[1] == std::vector<char>{ 'M', 'C' });
        CHECK(f.stacks[2] == std::vector<char>{ 'P', 'Z', 'N', 'D' });

        executeInstruction9001(f, input.instructions[2]);
        CHECK(f.stacks[0] == std::vector<char>{ 'M', 'C' });
        CHECK(f.stacks[1] == std::vector<char>{ });
        CHECK(f.stacks[2] == std::vector<char>{ 'P', 'Z', 'N', 'D' });

        executeInstruction9001(f, input.instructions[3]);
        CHECK(f.stacks[0] == std::vector<char>{ 'M' });
        CHECK(f.stacks[1] == std::vector<char>{ 'C' });
        CHECK(f.stacks[2] == std::vector<char>{ 'P', 'Z', 'N', 'D' });
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(input) == "MCD");
    }
}
