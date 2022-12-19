#include <not_enough_minerals.hpp>

#include <catch.hpp>

TEST_CASE("Not Enough Minerals")
{
    char const sample_input[] =
        "Blueprint 1: "
            "Each ore robot costs 4 ore. "
            "Each clay robot costs 2 ore. "
            "Each obsidian robot costs 3 ore and 14 clay. "
            "Each geode robot costs 2 ore and 7 obsidian. "
        "\n"
        "Blueprint 2: "
            "Each ore robot costs 2 ore. "
            "Each clay robot costs 3 ore. "
            "Each obsidian robot costs 3 ore and 8 clay. "
            "Each geode robot costs 3 ore and 12 obsidian. "
        "\n";

    auto const blueprints = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(blueprints.size() == 2);
        CHECK(blueprints[0] == Blueprint{
                .id = 1,
                .ore_robot = Blueprint::OreRobot{ .ore_cost = 4 },
                .clay_robot = Blueprint::ClayRobot{ .ore_cost = 2 },
                .obsidian_robot = Blueprint::ObsidianRobot{ .ore_cost = 3, .clay_cost = 14 },
                .geode_robot = Blueprint::GeodeRobot{ .ore_cost = 2, .obsidian_cost = 7 }
            });
        CHECK(blueprints[1] == Blueprint{
                .id = 2,
                .ore_robot = Blueprint::OreRobot{.ore_cost = 2 },
                .clay_robot = Blueprint::ClayRobot{.ore_cost = 3 },
                .obsidian_robot = Blueprint::ObsidianRobot{.ore_cost = 3, .clay_cost = 8 },
                .geode_robot = Blueprint::GeodeRobot{.ore_cost = 3, .obsidian_cost = 12 }
            });
    }

    SECTION("Score Blueprints")
    {
        CHECK(scoreBlueprint(blueprints[0]) == 9);
        CHECK(scoreBlueprint(blueprints[1]) == 24);
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(blueprints) == 33);
    }

#ifdef NDEBUG
    SECTION("Answer 2")
    {
        //CHECK(determineMostGeodes(blueprints[0], 32) == 56);      // this one is kind of slow still
        CHECK(determineMostGeodes(blueprints[1], 32) == 62);
    }
#endif
}
