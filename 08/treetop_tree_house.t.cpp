#include <treetop_tree_house.hpp>

#include <catch.hpp>

TEST_CASE("Treetop Tree House")
{
    char const sample_input[] =
        "30373" "\n"
        "25512" "\n"
        "65332" "\n"
        "33549" "\n"
        "35390" "\n";

    auto const field = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(field.span.extent(0) == 5);
        REQUIRE(field.span.extent(1) == 5);
        REQUIRE(field.data.size() == 25);

        CHECK(field.span(0, 0) == 3);
        CHECK(field.span(1, 0) == 0);
        CHECK(field.span(2, 0) == 3);
        CHECK(field.span(3, 0) == 7);
        CHECK(field.span(4, 0) == 3);

        CHECK(field.span(0, 1) == 2);
        CHECK(field.span(1, 1) == 5);
        CHECK(field.span(2, 1) == 5);
        CHECK(field.span(3, 1) == 1);
        CHECK(field.span(4, 1) == 2);

        CHECK(field.span(0, 2) == 6);
        CHECK(field.span(1, 2) == 5);
        CHECK(field.span(2, 2) == 3);
        CHECK(field.span(3, 2) == 3);
        CHECK(field.span(4, 2) == 2);

        CHECK(field.span(0, 3) == 3);
        CHECK(field.span(1, 3) == 3);
        CHECK(field.span(2, 3) == 5);
        CHECK(field.span(3, 3) == 4);
        CHECK(field.span(4, 3) == 9);

        CHECK(field.span(0, 4) == 3);
        CHECK(field.span(1, 4) == 5);
        CHECK(field.span(2, 4) == 3);
        CHECK(field.span(3, 4) == 9);
        CHECK(field.span(4, 4) == 0);
    }

    SECTION("Visible from left")
    {
        CHECK(visibleFromLeft(field, 0, 0));
        CHECK(visibleFromLeft(field, 1, 1));
        CHECK(!visibleFromLeft(field, 2, 1));
    }

    SECTION("Visible from top")
    {
        CHECK(visibleFromTop(field, 0, 0));
        CHECK(visibleFromTop(field, 1, 1));
        CHECK(!visibleFromTop(field, 1, 2));
    }

    SECTION("Visible from right")
    {
        CHECK(visibleFromRight(field, 4, 0));
        CHECK(!visibleFromRight(field, 3, 1));
        CHECK(visibleFromRight(field, 3, 2));
    }

    SECTION("Visible from bottom")
    {
        CHECK(visibleFromBottom(field, 4, 4));
        CHECK(!visibleFromBottom(field, 3, 3));
        CHECK(visibleFromBottom(field, 2, 3));
    }

    SECTION("Visibility")
    {
        CHECK(visibleFromLeft(field, 1, 1));
        CHECK(!visibleFromRight(field, 1, 1));
        CHECK(visibleFromTop(field, 1, 1));
        CHECK(!visibleFromBottom(field, 1, 1));
        CHECK(isVisible(field, 1, 1));

        CHECK(!visibleFromLeft(field, 2, 1));
        CHECK(visibleFromRight(field, 2, 1));
        CHECK(visibleFromTop(field, 2, 1));
        CHECK(!visibleFromBottom(field, 2, 1));
        CHECK(isVisible(field, 2, 1));

        CHECK(!visibleFromLeft(field, 3, 1));
        CHECK(!visibleFromRight(field, 3, 1));
        CHECK(!visibleFromTop(field, 3, 1));
        CHECK(!visibleFromBottom(field, 3, 1));
        CHECK(!isVisible(field, 3, 1));

        CHECK(!visibleFromLeft(field, 1, 2));
        CHECK(visibleFromRight(field, 1, 2));
        CHECK(!visibleFromTop(field, 1, 2));
        CHECK(!visibleFromBottom(field, 1, 2));
        CHECK(isVisible(field, 1, 2));

        CHECK(!visibleFromLeft(field, 2, 2));
        CHECK(!visibleFromRight(field, 2, 2));
        CHECK(!visibleFromTop(field, 2, 2));
        CHECK(!visibleFromBottom(field, 2, 2));
        CHECK(!isVisible(field, 2, 2));

        CHECK(!visibleFromLeft(field, 3, 2));
        CHECK(visibleFromRight(field, 3, 2));
        CHECK(!visibleFromTop(field, 3, 2));
        CHECK(!visibleFromBottom(field, 3, 2));
        CHECK(isVisible(field, 3, 2));

        CHECK(!isVisible(field, 1, 3));
        CHECK(isVisible(field, 2, 3));
        CHECK(!isVisible(field, 3, 3));
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(field) == 21);
    }

    SECTION("Count Visible")
    {
        CHECK(countLeft(field, 2, 1) == 1);
        CHECK(countRight(field, 2, 1) == 2);
        CHECK(countUp(field, 2, 1) == 1);
        CHECK(countDown(field, 2, 1) == 2);
        CHECK(scenicScore(field, 2, 1) == 4);

        CHECK(countLeft(field, 2, 3) == 2);
        CHECK(countRight(field, 2, 3) == 2);
        CHECK(countUp(field, 2, 3) == 2);
        CHECK(countDown(field, 2, 3) == 1);
        CHECK(scenicScore(field, 2, 3) == 8);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(field) == 8);
    }
}
