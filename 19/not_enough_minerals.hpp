#ifndef ADVENT_OF_CODE_19_NOT_ENOUGH_MINERALS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_19_NOT_ENOUGH_MINERALS_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <fmt/format.h>

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

struct Blueprint {
    int id;
    struct OreRobot {
        int ore_cost;
        friend bool operator==(OreRobot, OreRobot) = default;
    } ore_robot;
    struct ClayRobot {
        int ore_cost;
        friend bool operator==(ClayRobot, ClayRobot) = default;
    } clay_robot;
    struct ObsidianRobot {
        int ore_cost;
        int clay_cost;
        friend bool operator==(ObsidianRobot, ObsidianRobot) = default;
    } obsidian_robot;
    struct GeodeRobot {
        int ore_cost;
        int obsidian_cost;
        friend bool operator==(GeodeRobot, GeodeRobot) = default;
    } geode_robot;
    friend bool operator==(Blueprint const&, Blueprint const&) = default;
};

std::vector<Blueprint> parseInput(std::string_view input);

struct State {
    int ore_robot = 1;
    int clay_robot = 0;
    int obsidian_robot = 0;
    int geode_robot = 0;

    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geode = 0;

    int minutes_remain = 24;
};

bool canBuildOre(Blueprint const& b, State const& s);
bool canBuildClay(Blueprint const& b, State const& s);
bool canBuildObsidian(Blueprint const& b, State const& s);
bool canBuildGeode(Blueprint const& b, State const& s);

int scoreBlueprint(Blueprint const& b);

int answer1(std::vector<Blueprint> const& blueprints);

#endif
