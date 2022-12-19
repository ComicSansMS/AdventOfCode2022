#include <not_enough_minerals.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/numeric/accumulate.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <concepts>
#include <array>
#include <cassert>
#include <cmath>
#include <regex>


std::vector<Blueprint> parseInput(std::string_view input)
{
    std::regex rx_line(R"(Blueprint (\d+): Each ore robot costs (\d+) ore. )" 
                       R"(Each clay robot costs (\d+) ore. )"
                       R"(Each obsidian robot costs (\d+) ore and (\d+) clay. )"
                       R"(Each geode robot costs (\d+) ore and (\d+) obsidian.)");
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    auto parseLine = [](std::match_results<std::string_view::iterator> const& match) -> Blueprint {
        return Blueprint{
            .id = std::stoi(match[1]),
            .ore_robot = Blueprint::OreRobot{.ore_cost = std::stoi(match[2]) },
            .clay_robot = Blueprint::ClayRobot{.ore_cost = std::stoi(match[3]) },
            .obsidian_robot = Blueprint::ObsidianRobot{.ore_cost = std::stoi(match[4]), .clay_cost = std::stoi(match[5]) },
            .geode_robot = Blueprint::GeodeRobot{.ore_cost = std::stoi(match[6]), .obsidian_cost = std::stoi(match[7]) }
        };
    };

    return ranges::make_subrange(it_begin, it_end) | ranges::views::transform(parseLine) | ranges::to<std::vector>;
}

bool canBuildOre(Blueprint const& b, State const& s)
{
    return s.ore >= b.ore_robot.ore_cost;
}

bool canBuildClay(Blueprint const& b, State const& s)
{
    return s.ore >= b.clay_robot.ore_cost;
}

bool canBuildObsidian(Blueprint const& b, State const& s)
{
    return (s.ore >= b.obsidian_robot.ore_cost) && (s.clay >= b.obsidian_robot.clay_cost);
}

bool canBuildGeode(Blueprint const& b, State const& s)
{
    return (s.ore >= b.geode_robot.ore_cost) && (s.obsidian >= b.geode_robot.obsidian_cost);
}

State buildOre(Blueprint const& b, State s)
{
    ++s.ore_robot;
    s.ore -= b.ore_robot.ore_cost;
    assert(s.ore >= 0);
    return s;
}

State buildClay(Blueprint const& b, State s)
{
    ++s.clay_robot;
    s.ore -= b.clay_robot.ore_cost;
    assert(s.ore >= 0);
    return s;
}

State buildObsidian(Blueprint const& b, State s)
{
    ++s.obsidian_robot;
    s.ore -= b.obsidian_robot.ore_cost;
    assert(s.ore >= 0);
    s.clay -= b.obsidian_robot.clay_cost;
    assert(s.clay >= 0);
    return s;
}

State buildGeode(Blueprint const& b, State s)
{
    ++s.geode_robot;
    s.ore -= b.geode_robot.ore_cost;
    assert(s.ore >= 0);
    s.obsidian -= b.geode_robot.obsidian_cost;
    assert(s.obsidian >= 0);
    return s;
}

void collectMinerals(State& s)
{
    s.ore += s.ore_robot;
    s.clay += s.clay_robot;
    s.obsidian += s.obsidian_robot;
    s.geode += s.geode_robot;
    --s.minutes_remain;
}

int estimateMaxGeodes(State const& s)
{
    // all the current robots + 1 new robot built every minute
    return s.geode + (s.geode_robot * s.minutes_remain) + (((s.minutes_remain + 1) * s.minutes_remain) / 2);
}

int determineMostGeodes(Blueprint const& b)
{
    std::vector<State> stack;
    stack.emplace_back();
    int most_geodes = 0;
    fmt::print("ID {}\n", b.id);

    while (!stack.empty()) {
        State s = stack.back();
        stack.pop_back();
        assert(s.minutes_remain >= 0);
        if (s.minutes_remain == 0) {
            if (s.geode > most_geodes) { fmt::print("{}\n", s.geode); }
            most_geodes = std::max(most_geodes, s.geode);
            continue;
        }
        if (estimateMaxGeodes(s) <= most_geodes) {
            //fmt::print("Prune at {} with estimate {}\n", s.minutes_remain, estimateMaxGeodes(s));
            // prune path
            continue;
        }
        bool const can_build_ore = canBuildOre(b, s);
        bool const can_build_clay = canBuildClay(b, s);
        bool const can_build_obsidian = canBuildObsidian(b, s);
        bool const can_build_geode = canBuildGeode(b, s);
        collectMinerals(s);
        if (can_build_geode) { stack.emplace_back(buildGeode(b, s)); }
        if (can_build_obsidian) { stack.emplace_back(buildObsidian(b, s)); }
        if (can_build_clay) { stack.emplace_back(buildClay(b, s)); }
        if (can_build_ore) { stack.emplace_back(buildOre(b, s)); }
        stack.push_back(s);
    }

    return most_geodes;
}

int scoreBlueprint(Blueprint const& b)
{
    return determineMostGeodes(b) * b.id;
}

int answer1(std::vector<Blueprint> const& blueprints)
{
    return ranges::accumulate(blueprints, 0, ranges::plus{}, scoreBlueprint);
}

