#include <cathode_ray_tube.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Cathode-Ray Tube")
{
    char const sample_input1[] =
        "noop"    "\n"
        "addx 3"  "\n"
        "addx -5" "\n";

    char const sample_input2[] =
        "addx 15"   "\n"
        "addx -11"  "\n"
        "addx 6"    "\n"
        "addx -3"   "\n"
        "addx 5"    "\n"
        "addx -1"   "\n"
        "addx -8"   "\n"
        "addx 13"   "\n"
        "addx 4"    "\n"
        "noop"      "\n"
        "addx -1"   "\n"
        "addx 5"    "\n"
        "addx -1"   "\n"
        "addx 5"    "\n"
        "addx -1"   "\n"
        "addx 5"    "\n"
        "addx -1"   "\n"
        "addx 5"    "\n"
        "addx -1"   "\n"
        "addx -35"  "\n"
        "addx 1"    "\n"
        "addx 24"   "\n"
        "addx -19"  "\n"
        "addx 1"    "\n"
        "addx 16"   "\n"
        "addx -11"  "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 21"   "\n"
        "addx -15"  "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx -3"   "\n"
        "addx 9"    "\n"
        "addx 1"    "\n"
        "addx -3"   "\n"
        "addx 8"    "\n"
        "addx 1"    "\n"
        "addx 5"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx -36"  "\n"
        "noop"      "\n"
        "addx 1"    "\n"
        "addx 7"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 2"    "\n"
        "addx 6"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 7"    "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "addx -13"  "\n"
        "addx 13"   "\n"
        "addx 7"    "\n"
        "noop"      "\n"
        "addx 1"    "\n"
        "addx -33"  "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 2"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 8"    "\n"
        "noop"      "\n"
        "addx -1"   "\n"
        "addx 2"    "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "addx 17"   "\n"
        "addx -9"   "\n"
        "addx 1"    "\n"
        "addx 1"    "\n"
        "addx -3"   "\n"
        "addx 11"   "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx -13"  "\n"
        "addx -19"  "\n"
        "addx 1"    "\n"
        "addx 3"    "\n"
        "addx 26"   "\n"
        "addx -30"  "\n"
        "addx 12"   "\n"
        "addx -1"   "\n"
        "addx 3"    "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx -9"   "\n"
        "addx 18"   "\n"
        "addx 1"    "\n"
        "addx 2"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 9"    "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx -1"   "\n"
        "addx 2"    "\n"
        "addx -37"  "\n"
        "addx 1"    "\n"
        "addx 3"    "\n"
        "noop"      "\n"
        "addx 15"   "\n"
        "addx -21"  "\n"
        "addx 22"   "\n"
        "addx -6"   "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "addx 2"    "\n"
        "addx 1"    "\n"
        "noop"      "\n"
        "addx -10"  "\n"
        "noop"      "\n"
        "noop"      "\n"
        "addx 20"   "\n"
        "addx 1"    "\n"
        "addx 2"    "\n"
        "addx 2"    "\n"
        "addx -6"   "\n"
        "addx -11"  "\n"
        "noop"      "\n"
        "noop"      "\n"
        "noop"      "\n";

    auto const instructions1 = parseInput(sample_input1);
    auto const instructions2 = parseInput(sample_input2);

    SECTION("Parse Input")
    {
        CHECK(instructions1 == std::vector{
                Instruction{ Noop{} },
                Instruction{ AddX{ .val = 3 } },
                Instruction{ AddX{ .val = -5 } },
            });
        REQUIRE(instructions2.size() == 146);
        CHECK(instructions2[1] == Instruction{ AddX{ .val = -11 } });
    }

    SECTION("Compute Values X")
    {
        SECTION("Sample 1")
        {
            auto const xs = computeValuesX(instructions1);
            CHECK(xs == std::vector<std::int64_t>{
                1,  // cycle 0
                1,  // cycle 1
                1,  // cycle 2
                1,  // cycle 3
                4,  // cycle 4
                4,  // cycle 5
                -1, // cycle 6
            });
        }
        SECTION("Sample 2")
        {
            auto const xs = computeValuesX(instructions2);
            REQUIRE(xs.size() > 220);
            CHECK(xs[20] == 21);
            CHECK(xs[60] == 19);
            CHECK(xs[100] == 18);
            CHECK(xs[140] == 21);
            CHECK(xs[180] == 16);
            CHECK(xs[220] == 18);
        }
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(instructions2) == 13140);
    }

    SECTION("CRT")
    {
        CHECK(fmt::format("{}", CRT{}) ==
            "????????????????????????????????????????" "\n"
            "????????????????????????????????????????" "\n"
            "????????????????????????????????????????" "\n"
            "????????????????????????????????????????" "\n"
            "????????????????????????????????????????" "\n"
            "????????????????????????????????????????" "\n"
        );
    }

    SECTION("Answer 2")
    {
        CHECK(fmt::format("{}", answer2(instructions2)) ==
            "##..##..##..##..##..##..##..##..##..##.." "\n"
            "###...###...###...###...###...###...###." "\n"
            "####....####....####....####....####...." "\n"
            "#####.....#####.....#####.....#####....." "\n"
            "######......######......######......####" "\n"
            "#######.......#######.......#######....." "\n"
        );
    }
}
