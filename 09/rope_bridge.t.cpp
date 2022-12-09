#include <rope_bridge.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Rope Bridge")
{
    SECTION("Point to String")
    {
        std::stringstream sstr;
        sstr << Point{ .x = 42, .y = -23 };
        CHECK(sstr.str() == "(42, -23)");
    }

    char const sample_input[] =
        "R 4" "\n"
        "U 4" "\n"
        "L 3" "\n"
        "D 1" "\n"
        "R 4" "\n"
        "D 1" "\n"
        "L 5" "\n"
        "R 2" "\n";

    auto const instructions = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(instructions == std::vector<Instruction>{
            Instruction{ .direction = Direction::Right, .amount = 4 },
            Instruction{ .direction = Direction::Up, .amount = 4 },
            Instruction{ .direction = Direction::Left, .amount = 3 },
            Instruction{ .direction = Direction::Down, .amount = 1 },
            Instruction{ .direction = Direction::Right, .amount = 4 },
            Instruction{ .direction = Direction::Down, .amount = 1 },
            Instruction{ .direction = Direction::Left, .amount = 5 },
            Instruction{ .direction = Direction::Right, .amount = 2 },
        });
    }

    SECTION("Move Head Step")
    {
        SECTION("Move Right")
        {
            Rope<2> r;
            CHECK(r.head() == Point{ .x = 0, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Right);
            CHECK(r.head() == Point{ .x = 1, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Right);
            CHECK(r.head() == Point{ .x = 2, .y = 0 });
            CHECK(r.tail() == Point{ .x = 1, .y = 0 });
        }
        SECTION("Move Left")
        {
            Rope<2> r;
            CHECK(r.head() == Point{ .x = 0, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Left);
            CHECK(r.head() == Point{ .x = -1, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Left);
            CHECK(r.head() == Point{ .x = -2, .y = 0 });
            CHECK(r.tail() == Point{ .x = -1, .y = 0 });
        }
        SECTION("Move Up")
        {
            Rope<2> r;
            CHECK(r.head() == Point{ .x = 0, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Up);
            CHECK(r.head() == Point{ .x = 0, .y = 1 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Up);
            CHECK(r.head() == Point{ .x = 0, .y = 2 });
            CHECK(r.tail() == Point{ .x = 0, .y = 1 });
        }
        SECTION("Move Down")
        {
            Rope<2> r;
            CHECK(r.head() == Point{ .x = 0, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Down);
            CHECK(r.head() == Point{ .x = 0, .y = -1 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Down);
            CHECK(r.head() == Point{ .x = 0, .y = -2 });
            CHECK(r.tail() == Point{ .x = 0, .y = -1 });
        }
        SECTION("Move Diagonal Right Up")
        {
            Rope<2> r;
            CHECK(r.head() == Point{ .x = 0, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Right);
            CHECK(r.head() == Point{ .x = 1, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Up);
            CHECK(r.head() == Point{ .x = 1, .y = 1 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Up);
            CHECK(r.head() == Point{ .x = 1, .y = 2 });
            CHECK(r.tail() == Point{ .x = 1, .y = 1 });
        }
        SECTION("Move Diagonal Up Right")
        {
            Rope<2> r;
            CHECK(r.head() == Point{ .x = 0, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Right);
            CHECK(r.head() == Point{ .x = 1, .y = 0 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Up);
            CHECK(r.head() == Point{ .x = 1, .y = 1 });
            CHECK(r.tail() == Point{ .x = 0, .y = 0 });
            moveHead(r, Direction::Right);
            CHECK(r.head() == Point{ .x = 2, .y = 1 });
            CHECK(r.tail() == Point{ .x = 1, .y = 1 });
        }
    }

    SECTION("Move Head Instruction")
    {
        Rope<2> r;
        CHECK(r.head() == Point{ .x = 0, .y = 0 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        moveHead(r, instructions[0]);       // Right 4
        CHECK(r.head() == Point{ .x = 4, .y = 0 });
        CHECK(r.tail() == Point{ .x = 3, .y = 0 });
        moveHead(r, instructions[1]);       // Up 4
        CHECK(r.head() == Point{ .x = 4, .y = 4 });
        CHECK(r.tail() == Point{ .x = 4, .y = 3 });
        moveHead(r, instructions[2]);       // Left 3
        CHECK(r.head() == Point{ .x = 1, .y = 4 });
        CHECK(r.tail() == Point{ .x = 2, .y = 4 });
        moveHead(r, instructions[3]);       // Down 1
        CHECK(r.head() == Point{ .x = 1, .y = 3 });
        CHECK(r.tail() == Point{ .x = 2, .y = 4 });
        moveHead(r, instructions[4]);       // Right 4
        CHECK(r.head() == Point{ .x = 5, .y = 3 });
        CHECK(r.tail() == Point{ .x = 4, .y = 3 });
        moveHead(r, instructions[5]);       // Down 1
        CHECK(r.head() == Point{ .x = 5, .y = 2 });
        CHECK(r.tail() == Point{ .x = 4, .y = 3 });
        moveHead(r, instructions[6]);       // Left 5
        CHECK(r.head() == Point{ .x = 0, .y = 2 });
        CHECK(r.tail() == Point{ .x = 1, .y = 2 });
        moveHead(r, instructions[7]);       // Right 2
        CHECK(r.head() == Point{ .x = 2, .y = 2 });
        CHECK(r.tail() == Point{ .x = 1, .y = 2 });
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(instructions) == 13);
    }

    SECTION("Move Long Rope 1")
    {
        Rope<10> r;
        CHECK(r.head() == Point{ .x = 0, .y = 0 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "......" "\n"
            "......" "\n"
            "......" "\n"
            "H....." "\n"
        );
        moveHead(r, instructions[0]);       // Right 4
        CHECK(r.head() == Point{ .x = 4, .y = 0 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "......" "\n"
            "......" "\n"
            "......" "\n"
            "4321H." "\n"
        );
        //moveHead(r, instructions[1]);       // Up 4
        moveHead(r, Direction::Up);
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "......" "\n"
            "......" "\n"
            "....H." "\n"
            "4321.." "\n"
        );
        moveHead(r, Direction::Up);
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "......" "\n"
            "....H." "\n"
            ".4321." "\n"
            "5....." "\n"
        );
        moveHead(r, Direction::Up);
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "....H." "\n"
            "....1." "\n"
            ".432.." "\n"
            "5....." "\n"
        );
        moveHead(r, Direction::Up);
        CHECK(r.head() == Point{ .x = 4, .y = 4 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "....H." "\n"
            "....1." "\n"
            "..432." "\n"
            ".5...." "\n"
            "6....." "\n"
        );
        moveHead(r, instructions[2]);       // Left 3
        CHECK(r.head() == Point{ .x = 1, .y = 4 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            ".H1..." "\n"
            "...2.." "\n"
            "..43.." "\n"
            ".5...." "\n"
            "6....." "\n"
        );
        moveHead(r, instructions[3]);       // Down 1
        CHECK(r.head() == Point{ .x = 1, .y = 3 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "..1..." "\n"
            ".H.2.." "\n"
            "..43.." "\n"
            ".5...." "\n"
            "6....." "\n"
        );
        moveHead(r, instructions[4]);       // Right 4
        CHECK(r.head() == Point{ .x = 5, .y = 3 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "...21H" "\n"
            "..43.." "\n"
            ".5...." "\n"
            "6....." "\n"
        );
        moveHead(r, instructions[5]);       // Down 1
        CHECK(r.head() == Point{ .x = 5, .y = 2 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "...21." "\n"
            "..43.H" "\n"
            ".5...." "\n"
            "6....." "\n"
        );
        moveHead(r, instructions[6]);       // Left 5
        CHECK(r.head() == Point{ .x = 0, .y = 2 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "......" "\n"
            "H123.." "\n"
            ".5...." "\n"
            "6....." "\n"
        );
        moveHead(r, instructions[7]);       // Right 2
        CHECK(r.head() == Point{ .x = 2, .y = 2 });
        CHECK(r.tail() == Point{ .x = 0, .y = 0 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = 0, .max_x = 5, .min_y = 0, .max_y = 4, .r = r }) ==
            "......" "\n"
            "......" "\n"
            ".1H3.." "\n"
            ".5...." "\n"
            "6....." "\n"
        );
    }

    SECTION("Move Long Rope 2")
    {
        Rope<10> r;
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........H.............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Right, .amount = 5 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........54321H........." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Up, .amount = 8 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "................H........." "\n"
            "................1........." "\n"
            "................2........." "\n"
            "................3........." "\n"
            "...............54........." "\n"
            "..............6..........." "\n"
            ".............7............" "\n"
            "............8............." "\n"
            "...........9.............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Left, .amount = 8 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "........H1234............." "\n"
            "............5............." "\n"
            "............6............." "\n"
            "............7............." "\n"
            "............8............." "\n"
            "............9............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........s.............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Down, .amount = 3 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........2345............." "\n"
            "........1...6............." "\n"
            "........H...7............." "\n"
            "............8............." "\n"
            "............9............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........s.............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Right, .amount = 17 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "................987654321H" "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........s.............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Down, .amount = 10 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........s.........98765" "\n"
            ".........................4" "\n"
            ".........................3" "\n"
            ".........................2" "\n"
            ".........................1" "\n"
            ".........................H" "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Left, .amount = 25 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........s.............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "H123456789................" "\n"
        );
        moveHead(r, Instruction{ .direction = Direction::Up, .amount = 20 });
        CHECK(fmt::format("{}", Field<10>{ .min_x = -11, .max_x = 14, .min_y = -5, .max_y = 15, .r = r }) ==
            "H........................." "\n"
            "1........................." "\n"
            "2........................." "\n"
            "3........................." "\n"
            "4........................." "\n"
            "5........................." "\n"
            "6........................." "\n"
            "7........................." "\n"
            "8........................." "\n"
            "9........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            "...........s.............." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
            ".........................." "\n"
        );
        CHECK(r.visited.size() == 36);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(instructions) == 1);
        CHECK(answer2(std::vector<Instruction>{
            Instruction{ .direction = Direction::Right, .amount = 5 },
            Instruction{ .direction = Direction::Up, .amount = 8 },
            Instruction{ .direction = Direction::Left, .amount = 8 },
            Instruction{ .direction = Direction::Down, .amount = 3 },
            Instruction{ .direction = Direction::Right, .amount = 17 },
            Instruction{ .direction = Direction::Down, .amount = 10 },
            Instruction{ .direction = Direction::Left, .amount = 25 },
            Instruction{ .direction = Direction::Up, .amount = 20 },
        }) == 36);
    }
}
