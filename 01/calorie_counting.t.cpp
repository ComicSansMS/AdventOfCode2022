#include <calorie_counting.hpp>

#include <catch.hpp>

TEST_CASE("Calorie Counting")
{
    char const sample_input[] =
        R"(1000
        2000
        3000

        4000

        5000
        6000

        7000
        8000
        9000

        10000)";

    SECTION("Parse Input")
    {
        CHECK(parseInput("1\n2\n42") == std::vector<Elf>{ { .calories = {1, 2, 42} } });
        CHECK(parseInput("1\n2\n\n42") == std::vector<Elf>{ {.calories = { 1, 2 } }, { .calories = { 42 } } });
        CHECK(parseInput(sample_input) == std::vector<Elf>{
            { .calories = { 1000, 2000, 3000 } },
            { .calories = { 4000 } },
            { .calories = { 5000, 6000 } },
            { .calories = { 7000, 8000, 9000 } },
            { .calories = { 10000 } },
        });
    }

    auto const elves = parseInput(sample_input);

    SECTION("Total Calories")
    {
        CHECK(totalCalories(elves[0]) == 6000);
        CHECK(totalCalories(elves[1]) == 4000);
        CHECK(totalCalories(elves[2]) == 11000);
        CHECK(totalCalories(elves[3]) == 24000);
        CHECK(totalCalories(elves[4]) == 10000);
    }

    SECTION("Determine Max Calorie Elf")
    {
        CHECK(maxCalorieElf(std::vector<Elf>{ { .calories = { 1, 2, 42 } } }) == 0);
        CHECK(maxCalorieElf(std::vector<Elf>{ { .calories = { 1, 2 } }, { .calories = { 42 } } }) == 1);
        CHECK(maxCalorieElf(elves) == 3);
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(elves) == 24000);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(elves) == 45000);
    }
}
