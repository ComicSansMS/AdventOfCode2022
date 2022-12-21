#include <monkey_math.hpp>

#include <catch.hpp>

TEST_CASE("Monkey Math")
{
    char const sample_input[] =
        "root: pppw + sjmn" "\n"
        "dbpl: 5"           "\n"
        "cczh: sllz + lgvd" "\n"
        "zczc: 2"           "\n"
        "ptdq: humn - dvpt" "\n"
        "dvpt: 3"           "\n"
        "lfqf: 4"           "\n"
        "humn: 5"           "\n"
        "ljgn: 2"           "\n"
        "sjmn: drzm * dbpl" "\n"
        "sllz: 4"           "\n"
        "pppw: cczh / lfqf" "\n"
        "lgvd: ljgn * ptdq" "\n"
        "drzm: hmdt - zczc" "\n"
        "hmdt: 32"          "\n";

    auto monkeys = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(monkeys.size() == 15);
        CHECK(monkeys["root"].name == "root");
        CHECK(!monkeys["root"].value);
        REQUIRE(monkeys["root"].op);
        CHECK(monkeys["root"].op->lhs == "pppw");
        CHECK(monkeys["root"].op->op == Operation::Plus);
        CHECK(monkeys["root"].op->rhs == "sjmn");

        CHECK(monkeys["dbpl"].name == "dbpl");
        REQUIRE(monkeys["dbpl"].value);
        CHECK(monkeys["dbpl"].value.value() == 5);
        CHECK(!monkeys["dbpl"].op);

        CHECK(monkeys["zczc"].name == "zczc");
        REQUIRE(monkeys["zczc"].value);
        CHECK(monkeys["zczc"].value.value() == 2);
        CHECK(!monkeys["zczc"].op);

        CHECK(monkeys["humn"].name == "humn");
        REQUIRE(monkeys["humn"].value);
        CHECK(monkeys["humn"].value.value() == 5);
        CHECK(!monkeys["humn"].op);
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(monkeys) == 152);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(monkeys) == 301);
    }
}
