#include <monkey_in_the_middle.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Monkey in the Middle")
{
    char const sample_input[] =
        "Monkey 0:"                       "\n"
        "  Starting items: 79, 98"        "\n"
        "  Operation: new = old * 19"     "\n"
        "  Test: divisible by 23"         "\n"
        "    If true: throw to monkey 2"  "\n"
        "    If false: throw to monkey 3" "\n"
        ""                                "\n"
        "Monkey 1:"                       "\n"
        "  Starting items: 54, 65, 75, 74""\n"
        "  Operation: new = old + 6"      "\n"
        "  Test: divisible by 19"         "\n"
        "    If true: throw to monkey 2"  "\n"
        "    If false: throw to monkey 0" "\n"
        ""                                "\n"
        "Monkey 2:"                       "\n"
        "  Starting items: 79, 60, 97"    "\n"
        "  Operation: new = old * old"    "\n"
        "  Test: divisible by 13"         "\n"
        "    If true: throw to monkey 1"  "\n"
        "    If false: throw to monkey 3" "\n"
        ""                                "\n"
        "Monkey 3:"                       "\n"
        "  Starting items: 74"            "\n"
        "  Operation: new = old + 3"      "\n"
        "  Test: divisible by 17"         "\n"
        "    If true: throw to monkey 0"  "\n"
        "    If false: throw to monkey 1" "\n";

    auto const monkeys = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(monkeys.size() == 4);
        CHECK(monkeys[0].items == std::vector<std::int64_t>{ 79, 98 });
        CHECK(monkeys[0].operation == Operation{ .op = Op::Mul, .arg = 19 });
        CHECK(monkeys[0].test_divisor == 23);
        CHECK(monkeys[0].monkey_true == 2);
        CHECK(monkeys[0].monkey_false == 3);
        CHECK(monkeys[0].inspection_count == 0);

        CHECK(monkeys[1].items == std::vector<std::int64_t>{ 54, 65, 75, 74 });
        CHECK(monkeys[1].operation == Operation{ .op = Op::Add, .arg = 6 });
        CHECK(monkeys[1].test_divisor == 19);
        CHECK(monkeys[1].monkey_true == 2);
        CHECK(monkeys[1].monkey_false == 0);
        CHECK(monkeys[1].inspection_count == 0);

        CHECK(monkeys[2].items == std::vector<std::int64_t>{ 79, 60, 97 });
        CHECK(monkeys[2].operation == Operation{ .op = Op::Sqr, .arg = 0 });
        CHECK(monkeys[2].test_divisor == 13);
        CHECK(monkeys[2].monkey_true == 1);
        CHECK(monkeys[2].monkey_false == 3);
        CHECK(monkeys[2].inspection_count == 0);

        CHECK(monkeys[3].items == std::vector<std::int64_t>{ 74 });
        CHECK(monkeys[3].operation == Operation{ .op = Op::Add, .arg = 3 });
        CHECK(monkeys[3].test_divisor == 17);
        CHECK(monkeys[3].monkey_true == 0);
        CHECK(monkeys[3].monkey_false == 1);
        CHECK(monkeys[3].inspection_count == 0);
    }

    SECTION("Play Turn")
    {
        auto m = monkeys;
        playTurn(m, 0);
        CHECK(m[0].items.empty());
        CHECK(m[0].inspection_count == 2);
        CHECK(m[3].items == std::vector<std::int64_t>{ 74, 500, 620 });

        playTurn(m, 1);
        CHECK(m[1].items.empty());
        CHECK(m[1].inspection_count == 4);
        CHECK(m[0].items == std::vector<std::int64_t>{ 20, 23, 27, 26 });

        playTurn(m, 2);
        CHECK(m[2].items.empty());
        CHECK(m[2].inspection_count == 3);
        CHECK(m[1].items == std::vector<std::int64_t>{ 2080 });
        CHECK(m[3].items == std::vector<std::int64_t>{ 74, 500, 620, 1200, 3136 });

        playTurn(m, 3);
        CHECK(m[3].items.empty());
        CHECK(m[3].inspection_count == 5);
        CHECK(m[1].items == std::vector<std::int64_t>{ 2080, 25, 167, 207, 401, 1046 });
    }

    SECTION("Play Round")
    {
        auto m = monkeys;
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 20, 23, 27, 26 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 2080, 25, 167, 207, 401, 1046 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #2");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 695, 10, 71, 135, 350 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 43, 49, 58, 55, 362 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #3");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 16, 18, 21, 20, 122 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 1468, 22, 150, 286, 739 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #4");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 491, 9, 52, 97, 248, 34 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 39, 45, 43, 258 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #5");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 15, 17, 16, 88, 1037 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 20, 110, 205, 524, 72 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #6");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 8, 70, 176, 26, 34 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 481, 32, 36, 186, 2190 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #7");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 162, 12, 14, 64, 732, 17 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 148, 372, 55, 72 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #8");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 51, 126, 20, 26, 136 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 343, 26, 30, 1546, 36 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #9");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 116, 10, 12, 517, 14 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 108, 267, 43, 55, 288 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #10");
        playRound(m);
        CHECK(m[0].items == std::vector<std::int64_t>{ 91, 16, 20, 98 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 481, 245, 22, 26, 1092, 30 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #15");
        playRounds(m, 5);
        CHECK(m[0].items == std::vector<std::int64_t>{ 83, 44, 8, 184, 9, 20, 26, 102 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 110, 36 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        INFO("Round #20");
        playRounds(m, 5);
        CHECK(m[0].items == std::vector<std::int64_t>{ 10, 12, 14, 26, 34 });
        CHECK(m[1].items == std::vector<std::int64_t>{ 245, 93, 53, 199, 115 });
        CHECK(m[2].items == std::vector<std::int64_t>{ });
        CHECK(m[3].items == std::vector<std::int64_t>{ });

        CHECK(m[0].inspection_count == 101);
        CHECK(m[1].inspection_count == 95);
        CHECK(m[2].inspection_count == 7);
        CHECK(m[3].inspection_count == 105);
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(monkeys) == 10605);
    }

    SECTION("Play Rounds No Decay")
    {
        auto m = monkeys;
        auto const neutral_decay = determineNeutralDecay(m);
        
        INFO("Round #1");
        playRounds(m, 1, neutral_decay);
        CHECK(m[0].inspection_count == 2);
        CHECK(m[1].inspection_count == 4);
        CHECK(m[2].inspection_count == 3);
        CHECK(m[3].inspection_count == 6);
        
        INFO("Round #20");
        playRounds(m, 19, neutral_decay);
        CHECK(m[0].inspection_count == 99);
        CHECK(m[1].inspection_count == 97);
        CHECK(m[2].inspection_count == 8);
        CHECK(m[3].inspection_count == 103);

        INFO("Round #1000");
        playRounds(m, 980, neutral_decay);
        CHECK(m[0].inspection_count == 5204);
        CHECK(m[1].inspection_count == 4792);
        CHECK(m[2].inspection_count == 199);
        CHECK(m[3].inspection_count == 5192);

        INFO("Round #2000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 10419);
        CHECK(m[1].inspection_count == 9577);
        CHECK(m[2].inspection_count == 392);
        CHECK(m[3].inspection_count == 10391);

        INFO("Round #3000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 15638);
        CHECK(m[1].inspection_count == 14358);
        CHECK(m[2].inspection_count == 587);
        CHECK(m[3].inspection_count == 15593);

        INFO("Round #4000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 20858);
        CHECK(m[1].inspection_count == 19138);
        CHECK(m[2].inspection_count == 780);
        CHECK(m[3].inspection_count == 20797);

        INFO("Round #5000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 26075);
        CHECK(m[1].inspection_count == 23921);
        CHECK(m[2].inspection_count == 974);
        CHECK(m[3].inspection_count == 26000);

        INFO("Round #6000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 31294);
        CHECK(m[1].inspection_count == 28702);
        CHECK(m[2].inspection_count == 1165);
        CHECK(m[3].inspection_count == 31204);

        INFO("Round #7000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 36508);
        CHECK(m[1].inspection_count == 33488);
        CHECK(m[2].inspection_count == 1360);
        CHECK(m[3].inspection_count == 36400);

        INFO("Round #8000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 41728);
        CHECK(m[1].inspection_count == 38268);
        CHECK(m[2].inspection_count == 1553);
        CHECK(m[3].inspection_count == 41606);

        INFO("Round #9000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 46945);
        CHECK(m[1].inspection_count == 43051);
        CHECK(m[2].inspection_count == 1746);
        CHECK(m[3].inspection_count == 46807);

        INFO("Round #10000");
        playRounds(m, 1000, neutral_decay);
        CHECK(m[0].inspection_count == 52166);
        CHECK(m[1].inspection_count == 47830);
        CHECK(m[2].inspection_count == 1938);
        CHECK(m[3].inspection_count == 52013);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(monkeys) == 2713310158);
    }
}
