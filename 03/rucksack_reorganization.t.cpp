#include <rucksack_reorganization.hpp>

#include <catch.hpp>

TEST_CASE("Rucksack Reorganization")
{
    char const sample_input[] =
        "vJrwpWtwJgWrhcsFMMfFFhFp"         "\n"
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL" "\n"
        "PmmdzqPrVvPwwTWBwg"               "\n"
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn"   "\n"
        "ttgJtRGJQctTZtZT"                 "\n"
        "CrZsJsPPZsGzwwsLwLmpwMDw"         "\n";

    SECTION("prio")
    {
        CHECK(prio('a') == 1);
        CHECK(prio('b') == 2);
        CHECK(prio('c') == 3);
        CHECK(prio('y') == 25);
        CHECK(prio('z') == 26);
        CHECK(prio('A') == 27);
        CHECK(prio('B') == 28);
        CHECK(prio('C') == 29);
        CHECK(prio('Y') == 51);
        CHECK(prio('Z') == 52);
    }


    auto const rucksacks = parseInput(sample_input);


    SECTION("Parse Input")
    {
        REQUIRE(rucksacks.size() == 6);
        // vJrwpWtwJgWr
        CHECK(rucksacks[0].first[prio('a')] == 0);
        CHECK(rucksacks[0].first[prio('A')] == 0);
        CHECK(rucksacks[0].first[prio('v')] == 1);
        CHECK(rucksacks[0].first[prio('J')] == 2);
        CHECK(rucksacks[0].first[prio('r')] == 2);
        CHECK(rucksacks[0].first[prio('w')] == 2);
        CHECK(rucksacks[0].first[prio('p')] == 1);
        CHECK(rucksacks[0].first[prio('W')] == 2);
        CHECK(rucksacks[0].first[prio('g')] == 1);
        CHECK(rucksacks[0].first[prio('r')] == 2);
        // hcsFMMfFFhFp
        CHECK(rucksacks[0].second[prio('h')] == 2);
        CHECK(rucksacks[0].second[prio('c')] == 1);
        CHECK(rucksacks[0].second[prio('s')] == 1);
        CHECK(rucksacks[0].second[prio('F')] == 4);
        CHECK(rucksacks[0].second[prio('M')] == 2);
        CHECK(rucksacks[0].second[prio('f')] == 1);
        CHECK(rucksacks[0].second[prio('p')] == 1);
    }

    SECTION("Common Item")
    {
        CHECK(commonItem(Rucksack{}) == 0);
        CHECK(commonItem(rucksacks[0]) == 16);
        CHECK(commonItem(rucksacks[1]) == 38);
        CHECK(commonItem(rucksacks[2]) == 42);
        CHECK(commonItem(rucksacks[3]) == 22);
        CHECK(commonItem(rucksacks[4]) == 20);
        CHECK(commonItem(rucksacks[5]) == 19);
        CHECK(answer1(rucksacks) == 157);
    }

    SECTION("Common Group Item")
    {
        CHECK(commonGroupItem(Rucksack{}, Rucksack{}, Rucksack{}) == 0);
        CHECK(commonGroupItem(rucksacks[0], rucksacks[1], rucksacks[2]) == 18);
        CHECK(commonGroupItem(rucksacks[3], rucksacks[4], rucksacks[5]) == 52);
        CHECK(answer2(rucksacks) == 70);
    }
}
