#include <rock_paper_scissors.hpp>

#include <catch.hpp>

TEST_CASE("Rock Paper Scissors")
{
    char const sample_input[] =
        "A Y" "\n"
        "B X" "\n"
        "C Z" "\n";

    auto const turns = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(turns.size() == 3);
        CHECK(turns == std::vector<Turn>{
            { .opponent = { Play::Rock }, .second = { Second::Y } },
            { .opponent = { Play::Paper }, .second = { Second::X } },
            { .opponent = { Play::Scissors }, .second = { Second::Z } },
        });
    }

    auto decoded_rounds1 = decodeTurn(turns);

    SECTION("Decode Turn")
    {
        CHECK(decoded_rounds1 == std::vector<Round>{
            {.opponent = { Play::Rock }, .me = { Play::Paper } },
            {.opponent = { Play::Paper }, .me = { Play::Rock } },
            {.opponent = { Play::Scissors }, .me = { Play::Scissors } },
        });
    }

    SECTION("Calculate Score")
    {
        CHECK(calculateScore(decoded_rounds1[0]) == 8);
        CHECK(calculateScore(decoded_rounds1[1]) == 1);
        CHECK(calculateScore(decoded_rounds1[2]) == 6);
        CHECK(totalScore(decoded_rounds1) == 15);
    }

    SECTION("Decode Turn Proper")
    {
        auto decoded_rounds2 = decodeTurnProper(turns);
        CHECK(decoded_rounds2 == std::vector<Round>{
            {.opponent = { Play::Rock }, .me = { Play::Rock } },
            { .opponent = { Play::Paper }, .me = { Play::Rock } },
            { .opponent = { Play::Scissors }, .me = { Play::Rock } },
        });
        CHECK(totalScore(decoded_rounds2) == 12);
    }
}
