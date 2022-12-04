#include <camp_cleanup.hpp>

#include <catch.hpp>

TEST_CASE("Camp Cleanup")
{
    char const sample_input[] =
        "2-4,6-8" "\n"
        "2-3,4-5" "\n"
        "5-7,7-9" "\n"
        "2-8,3-7" "\n"
        "6-6,4-6" "\n"
        "2-6,4-8" "\n";

    auto const assignments = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(assignments.size() == 6);
        CHECK(assignments[0].first.first == 2);
        CHECK(assignments[0].first.last == 4);
        CHECK(assignments[0].second.first == 6);
        CHECK(assignments[0].second.last == 8);
        CHECK(assignments[1] == Assignment{ .first = { .first = 2, .last = 3 },
                                            .second = { .first = 4, .last = 5 } });
        CHECK(assignments[2] == Assignment{ .first = {.first = 5, .last = 7 },
                                            .second = {.first = 7, .last = 9 } });
        CHECK(assignments[3] == Assignment{ .first = {.first = 2, .last = 8 },
                                            .second = {.first = 3, .last = 7 } });
        CHECK(assignments[4] == Assignment{ .first = {.first = 6, .last = 6 },
                                            .second = {.first = 4, .last = 6 } });
        CHECK(assignments[5] == Assignment{ .first = {.first = 2, .last = 6 },
                                            .second = {.first = 4, .last = 8 } });

        CHECK(parseInput("52-64,999-12324\n") ==
            std::vector{ Assignment{ .first = { .first = 52, .last = 64 },
                                     .second = { .first = 999, .last = 12324 } } });
    }

    SECTION("Fully Contained Range")
    {
        CHECK(!fullyContainedRange(assignments[0]));
        CHECK(!fullyContainedRange(assignments[1]));
        CHECK(!fullyContainedRange(assignments[2]));
        CHECK(fullyContainedRange(assignments[3]));
        CHECK(fullyContainedRange(assignments[4]));
        CHECK(!fullyContainedRange(assignments[5]));
        CHECK(answer1(assignments) == 2);
    }

    SECTION("Range Overlap")
    {
        CHECK(!doesOverlap(assignments[0]));
        CHECK(!doesOverlap(assignments[1]));
        CHECK(doesOverlap(assignments[2]));
        CHECK(doesOverlap(assignments[3]));
        CHECK(doesOverlap(assignments[4]));
        CHECK(doesOverlap(assignments[5]));
        CHECK(answer2(assignments) == 4);
    }
}
