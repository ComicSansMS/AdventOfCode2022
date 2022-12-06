#include <tuning_trouble.hpp>

#include <string>

#include <catch.hpp>

TEST_CASE("Supply Stacks")
{
    std::string const sample_data[5] = {
        "mjqjpqmgbljsphdztnvjfqwrcgsmlb",
        "bvwbjplbgvbhsrlpgdmjqwftvncz",
        "nppdvjthqldpwncqszvftbrmjlhg",
        "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg",
        "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"
    };

    SECTION("Parse Input")
    {
        CHECK(parseInput("mjqjpqmgbljsphdztnvjfqwrcgsmlb" "\n") == "mjqjpqmgbljsphdztnvjfqwrcgsmlb");
    }

    SECTION("Find Marker")
    {
        CHECK(findMarker(sample_data[0], 4) == sample_data[0].begin() + 7);
        CHECK(findMarker(sample_data[1], 4) == sample_data[1].begin() + 5);
        CHECK(findMarker(sample_data[2], 4) == sample_data[2].begin() + 6);
        CHECK(findMarker(sample_data[3], 4) == sample_data[3].begin() + 10);
        CHECK(findMarker(sample_data[4], 4) == sample_data[4].begin() + 11);

        CHECK(findMarker(sample_data[0], 14) == sample_data[0].begin() + 19);
        CHECK(findMarker(sample_data[1], 14) == sample_data[1].begin() + 23);
        CHECK(findMarker(sample_data[2], 14) == sample_data[2].begin() + 23);
        CHECK(findMarker(sample_data[3], 14) == sample_data[3].begin() + 29);
        CHECK(findMarker(sample_data[4], 14) == sample_data[4].begin() + 26);
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(sample_data[0]) == 7);
        CHECK(answer1(sample_data[1]) == 5);
        CHECK(answer1(sample_data[2]) == 6);
        CHECK(answer1(sample_data[3]) == 10);
        CHECK(answer1(sample_data[4]) == 11);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(sample_data[0]) == 19);
        CHECK(answer2(sample_data[1]) == 23);
        CHECK(answer2(sample_data[2]) == 23);
        CHECK(answer2(sample_data[3]) == 29);
        CHECK(answer2(sample_data[4]) == 26);
    }
}
