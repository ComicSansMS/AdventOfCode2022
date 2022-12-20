#include <grove_positioning_system.hpp>

#include <catch.hpp>

TEST_CASE("Grove Positioning System")
{
    char const sample_input[] =
        "1"  "\n"
        "2"  "\n"
        "-3" "\n"
        "3"  "\n"
        "-2" "\n"
        "0"  "\n"
        "4"  "\n";

    auto const numbers = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(numbers == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4 });
    }

    SECTION("Decode Step")
    {
        {
            auto l2 = std::list<int64_t>{ 4, 5, 6, 1, 7, 8, 9 };
            auto it = l2.begin();
            std::advance(it, 3);
            REQUIRE(*it == 1);
            decodeStep(l2, Tracker{ .i = *it, .it = it});
            CHECK(l2 == std::list<int64_t>{ 4, 5, 6, 7, 1, 8, 9 });
        }
        {
            auto l2 = std::list<int64_t>{ 4, -2, 5, 6, 7, 8, 9 };
            auto it = l2.begin();
            std::advance(it, 1);
            REQUIRE(*it == -2);
            decodeStep(l2, Tracker{ .i = *it, .it = it });
            CHECK(l2 == std::list<int64_t>{ 4, 5, 6, 7, 8, -2, 9 });
        }
        {
            auto l2 = std::list<int64_t>{ 4, -2, 5, 6, 7, 1, 9 };
            auto it = l2.begin();
            std::advance(it, 5);
            REQUIRE(*it == 1);
            decodeStep(l2, Tracker{ .i = *it, .it = it });
            CHECK(l2 == std::list<int64_t>{ 1, 4, -2, 5, 6, 7, 9 });
        }
        {
            auto l2 = std::list<int64_t>{ 4, -2, 5, 6, 7, 2, 9 };
            auto it = l2.begin();
            std::advance(it, 5);
            REQUIRE(*it == 2);
            decodeStep(l2, Tracker{ .i = *it, .it = it });
            CHECK(l2 == std::list<int64_t>{ 4, 2, -2, 5, 6, 7, 9 });
        }
        {
            auto l2 = std::list<int64_t>{ 5, 2, -3, 3, -2, 0, 4 };
            decodeStep(l2, Tracker{ .i = 5, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 2, -3, 3, -2, 0, 5, 4 });
        }
        {
            auto l2 = std::list<int64_t>{ 6, 2, -3, 3, -2, 0, 4 };
            decodeStep(l2, Tracker{ .i = 6, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 6, 2, -3, 3, -2, 0, 4 });
        }
        {
            auto l2 = std::list<int64_t>{ 7, 2, -3, 3, -2, 0, 4 };
            decodeStep(l2, Tracker{ .i = 7, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 2, 7, -3, 3, -2, 0, 4 });
        }
        {
            auto l2 = std::list<int64_t>{ 8, 2, -3, 3, -2, 0, 4 };
            decodeStep(l2, Tracker{ .i = 8, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 2, -3, 8, 3, -2, 0, 4 });
        }
        {
            auto l2 = std::list<int64_t>{ 2, 5, -3, 3, -2, 0, 4 };
            auto it = l2.begin();
            std::advance(it, 1);
            REQUIRE(*it == 5);
            decodeStep(l2, Tracker{ .i = *it, .it = it });
            CHECK(l2 == std::list<int64_t>{ 5, 2, -3, 3, -2, 0, 4 });
        }
        {
            auto l2 = numbers;
            CHECK(l2 == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4 });
            decodeStep(l2, Tracker{ .i = -1, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 2, -3, 3, -2, 0, 1, 4, });
        }
        {
            auto l2 = numbers;
            CHECK(l2 == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4 });
            decodeStep(l2, Tracker{ .i = -2, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 2, -3, 3, -2, 1, 0, 4, });
        }
        {
            auto l2 = numbers;
            CHECK(l2 == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4 });
            decodeStep(l2, Tracker{ .i = -5, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 2, 1, -3, 3, -2, 0, 4, });
        }
        {
            auto l2 = numbers;
            CHECK(l2 == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4 });
            decodeStep(l2, Tracker{ .i = -6, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4, });
        }
        {
            auto l2 = numbers;
            CHECK(l2 == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4 });
            decodeStep(l2, Tracker{ .i = -7, .it = l2.begin() });
            CHECK(l2 == std::list<int64_t>{ 2, -3, 3, -2, 0, 1, 4, });
        }
        auto l = numbers;
        auto const t = buildTracker(l);
        REQUIRE(t.size() == 7);
        CHECK(l == std::list<int64_t>{ 1, 2, -3, 3, -2, 0, 4 });
        // 1 moves between 2 and -3:
        decodeStep(l, t[0]);
        CHECK(l == std::list<int64_t>{ 2, 1, -3, 3, -2, 0, 4 });
        // 2 moves between -3 and 3:
        decodeStep(l, t[1]);
        CHECK(l == std::list<int64_t>{ 1, -3, 2, 3, -2, 0, 4 });
        // -3 moves between -2 and 0:
        decodeStep(l, t[2]);
        CHECK(l == std::list<int64_t>{ 1, 2, 3, -2, -3, 0, 4 });
        // 3 moves between 0 and 4:
        decodeStep(l, t[3]);
        CHECK(l == std::list<int64_t>{ 1, 2, -2, -3, 0, 3, 4 });
        // -2 moves between 4 and 1:
        decodeStep(l, t[4]);
        CHECK(l == std::list<int64_t>{ 1, 2, -3, 0, 3, 4, -2 });
        // 0 does not move:
        decodeStep(l, t[5]);
        CHECK(l == std::list<int64_t>{ 1, 2, -3, 0, 3, 4, -2 });
        // 4 moves between -3 and 0:
        decodeStep(l, t[6]);
        CHECK(l == std::list<int64_t>{ 1, 2, -3, 4, 0, 3, -2 });
    }

    SECTION("Decode")
    {
        auto l = numbers;
        decode(l);
        CHECK(l == std::list<int64_t>{ 1, 2, -3, 4, 0, 3, -2 });
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(numbers) == 3);
    }

    SECTION("Decode 2")
    {
        auto l = applyDecryptionKey(numbers);
        auto const trackers = buildTracker(l);
        CHECK(l == std::list<int64_t>{ 811589153, 1623178306, -2434767459, 2434767459, -1623178306, 0, 3246356612 });
        // round 1
        decode(l, trackers);
        CHECK(l == std::list<int64_t>{ 0, -2434767459, 3246356612, -1623178306, 2434767459, 1623178306, 811589153 });
        // round 2
        decode(l, trackers);
        CHECK(l == std::list<int64_t>{ 0, 2434767459, 1623178306, 3246356612, -2434767459, -1623178306, 811589153 });
    }

    SECTION("Decode 2")
    {
        auto l = applyDecryptionKey(numbers);
        decode2(l);
        CHECK(l == std::list<int64_t>{ -2434767459, 1623178306, 3246356612, -1623178306, 2434767459, 811589153, 0 });
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(numbers) == 1623178306);
    }

}
