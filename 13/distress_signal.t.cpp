#include <distress_signal.hpp>

#include <catch.hpp>

TEST_CASE("Distress Signal")
{
    char const sample_input[] =
        "[1,1,3,1,1]"                 "\n"
        "[1,1,5,1,1]"                 "\n"
        ""                            "\n"
        "[[1],[2,3,4]]"               "\n"
        "[[1],4]"                     "\n"
        ""                            "\n"
        "[9]"                         "\n"
        "[[8,7,6]]"                   "\n"
        ""                            "\n"
        "[[4,4],4,4]"                 "\n"
        "[[4,4],4,4,4]"               "\n"
        ""                            "\n"
        "[7,7,7,7]"                   "\n"
        "[7,7,7]"                     "\n"
        ""                            "\n"
        "[]"                          "\n"
        "[3]"                         "\n"
        ""                            "\n"
        "[[[]]]"                      "\n"
        "[[]]"                        "\n"
        ""                            "\n"
        "[1,[2,[3,[4,[5,6,7]]]],8,9]" "\n"
        "[1,[2,[3,[4,[5,6,0]]]],8,9]" "\n";

    auto const data = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(data.size() == 8);
        // [1,1,3,1,1]
        REQUIRE(data[0].lhs.elements.size() == 5);
        CHECK(data[0].lhs.elements[0].asInt() == 1);
        CHECK(data[0].lhs.elements[1].asInt() == 1);
        CHECK(data[0].lhs.elements[2].asInt() == 3);
        CHECK(data[0].lhs.elements[3].asInt() == 1);
        CHECK(data[0].lhs.elements[4].asInt() == 1);
        // [1,1,5,1,1]
        REQUIRE(data[0].rhs.elements.size() == 5);
        CHECK(data[0].rhs.elements[0].asInt() == 1);
        CHECK(data[0].rhs.elements[1].asInt() == 1);
        CHECK(data[0].rhs.elements[2].asInt() == 5);
        CHECK(data[0].rhs.elements[3].asInt() == 1);
        CHECK(data[0].rhs.elements[4].asInt() == 1);

        // [[1],[2,3,4]]
        REQUIRE(data[1].lhs.elements.size() == 2);
        CHECK(isList(data[1].lhs.elements[0]));
        REQUIRE(data[1].lhs.elements[0].asList().elements.size() == 1);
        CHECK(data[1].lhs.elements[0].asList().elements[0].asInt() == 1);
        CHECK(isList(data[1].lhs.elements[1]));
        REQUIRE(data[1].lhs.elements[1].asList().elements.size() == 3);
        CHECK(data[1].lhs.elements[1].asList().elements[0].asInt() == 2);
        CHECK(data[1].lhs.elements[1].asList().elements[1].asInt() == 3);
        CHECK(data[1].lhs.elements[1].asList().elements[2].asInt() == 4);

        // [[1],4]
        REQUIRE(data[1].rhs.elements.size() == 2);
        CHECK(isList(data[1].rhs.elements[0]));
        REQUIRE(data[1].rhs.elements[0].asList().elements.size() == 1);
        CHECK(data[1].rhs.elements[0].asList().elements[0].asInt() == 1);
        CHECK(data[1].rhs.elements[1].asInt() == 4);

        // [9]
        REQUIRE(data[2].lhs.elements.size() == 1);
        CHECK(data[2].lhs.elements[0].asInt() == 9);
        // [[8,7,6]]
        REQUIRE(data[2].rhs.elements.size() == 1);
        REQUIRE(data[2].rhs.elements[0].asList().elements.size() == 3);
        CHECK(data[2].rhs.elements[0].asList().elements[0].asInt() == 8);
        CHECK(data[2].rhs.elements[0].asList().elements[1].asInt() == 7);
        CHECK(data[2].rhs.elements[0].asList().elements[2].asInt() == 6);

        // [[4,4],4,4]
        REQUIRE(data[3].lhs.elements.size() == 3);
        REQUIRE(data[3].lhs.elements[0].asList().elements.size() == 2);
        CHECK(data[3].lhs.elements[0].asList().elements[0].asInt() == 4);
        CHECK(data[3].lhs.elements[0].asList().elements[1].asInt() == 4);
        CHECK(data[3].lhs.elements[1].asInt() == 4);
        CHECK(data[3].lhs.elements[2].asInt() == 4);
        // [[4,4],4,4,4]
        REQUIRE(data[3].rhs.elements.size() == 4);
        REQUIRE(data[3].rhs.elements[0].asList().elements.size() == 2);
        CHECK(data[3].rhs.elements[0].asList().elements[0].asInt() == 4);
        CHECK(data[3].rhs.elements[0].asList().elements[1].asInt() == 4);
        CHECK(data[3].rhs.elements[1].asInt() == 4);
        CHECK(data[3].rhs.elements[2].asInt() == 4);
        CHECK(data[3].rhs.elements[3].asInt() == 4);

        // [7,7,7,7]
        REQUIRE(data[4].lhs.elements.size() == 4);
        CHECK(data[4].lhs.elements[0].asInt() == 7);
        CHECK(data[4].lhs.elements[1].asInt() == 7);
        CHECK(data[4].lhs.elements[2].asInt() == 7);
        CHECK(data[4].lhs.elements[3].asInt() == 7);
        // [7,7,7]
        REQUIRE(data[4].rhs.elements.size() == 3);
        CHECK(data[4].rhs.elements[0].asInt() == 7);
        CHECK(data[4].rhs.elements[1].asInt() == 7);
        CHECK(data[4].rhs.elements[2].asInt() == 7);

        // []
        CHECK(data[5].lhs.elements.empty());
        // [3]
        REQUIRE(data[5].rhs.elements.size() == 1);
        CHECK(data[5].rhs.elements[0].asInt() == 3);

        // [[[]]]
        REQUIRE(data[6].lhs.elements.size() == 1);
        REQUIRE(data[6].lhs.elements[0].asList().elements.size() == 1);
        CHECK(data[6].lhs.elements[0].asList().elements[0].asList().elements.empty());
        // [[]]
        REQUIRE(data[6].rhs.elements.size() == 1);
        CHECK(data[6].rhs.elements[0].asList().elements.empty());

        // [1,[2,[3,[4,[5,6,7]]]],8,9]
        REQUIRE(data[7].lhs.elements.size() == 4);
        CHECK(data[7].lhs.elements[0].asInt() == 1);
        REQUIRE(data[7].lhs.elements[1].asList().elements.size() == 2);
        CHECK(data[7].lhs.elements[1].asList().elements[0].asInt() == 2);
        REQUIRE(data[7].lhs.elements[1].asList().elements[1].asList().elements.size() == 2);
        CHECK(data[7].lhs.elements[1].asList().elements[1].asList().elements[0].asInt() == 3);
        REQUIRE(data[7].lhs.elements[1].asList().elements[1].asList().elements[1].asList().elements.size() == 2);
        CHECK(data[7].lhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[0].asInt() == 4);
        REQUIRE(data[7].lhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements.size() == 3);
        CHECK(data[7].lhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements[0].asInt() == 5);
        CHECK(data[7].lhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asInt() == 6);
        CHECK(data[7].lhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements[2].asInt() == 7);
        CHECK(data[7].lhs.elements[2].asInt() == 8);
        CHECK(data[7].lhs.elements[3].asInt() == 9);
        // [1,[2,[3,[4,[5,6,0]]]],8,9]
        REQUIRE(data[7].rhs.elements.size() == 4);
        CHECK(data[7].rhs.elements[0].asInt() == 1);
        REQUIRE(data[7].rhs.elements[1].asList().elements.size() == 2);
        CHECK(data[7].rhs.elements[1].asList().elements[0].asInt() == 2);
        REQUIRE(data[7].rhs.elements[1].asList().elements[1].asList().elements.size() == 2);
        CHECK(data[7].rhs.elements[1].asList().elements[1].asList().elements[0].asInt() == 3);
        REQUIRE(data[7].rhs.elements[1].asList().elements[1].asList().elements[1].asList().elements.size() == 2);
        CHECK(data[7].rhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[0].asInt() == 4);
        REQUIRE(data[7].rhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements.size() == 3);
        CHECK(data[7].rhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements[0].asInt() == 5);
        CHECK(data[7].rhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asInt() == 6);
        CHECK(data[7].rhs.elements[1].asList().elements[1].asList().elements[1].asList().elements[1].asList().elements[2].asInt() == 0);
        CHECK(data[7].rhs.elements[2].asInt() == 8);
        CHECK(data[7].rhs.elements[3].asInt() == 9);
    }

    SECTION("Check Order")
    {
        CHECK(checkOrder(data[0]));
        CHECK(checkOrder(data[1]));
        CHECK(!checkOrder(data[2]));
        CHECK(checkOrder(data[3]));
        CHECK(!checkOrder(data[4]));
        CHECK(checkOrder(data[5]));
        CHECK(!checkOrder(data[6]));
        CHECK(!checkOrder(data[7]));
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(data) == 13);
    }

    SECTION("Decoder Packets")
    {
        auto p1 = createDecoderPacket1();
        REQUIRE(p1.elements.size() == 1);
        REQUIRE(p1.elements[0].asList().elements.size() == 1);
        CHECK(p1.elements[0].asList().elements[0].asInt() == 2);

        auto p2 = createDecoderPacket2();
        REQUIRE(p2.elements.size() == 1);
        REQUIRE(p2.elements[0].asList().elements.size() == 1);
        CHECK(p2.elements[0].asList().elements[0].asInt() == 6);

        CHECK(isDecoderPacket1(p1));
        CHECK(!isDecoderPacket1(p2));
        CHECK(!isDecoderPacket2(p1));
        CHECK(isDecoderPacket2(p2));
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(parseInput(sample_input)) == 140);
    }
}
