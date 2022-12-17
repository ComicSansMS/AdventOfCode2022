#ifndef ADVENT_OF_CODE_16_PROBOSCIDEA_VOLCANIUM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_16_PROBOSCIDEA_VOLCANIUM_HPP_INCLUDE_GUARD

#include <experimental/mdspan>

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Valve {
    std::string name;
    int flow_rate;
    std::vector<std::string> connections;

    friend bool operator==(Valve const&, Valve const&) = default;
};

std::vector<Valve> parseInput(std::string_view input);

std::string toDot(std::vector<Valve> const& valves);

struct Matrix {
    std::vector<int> m;
    int dimension;
};

using ValveIndexMap = std::unordered_map<std::string, int>;
ValveIndexMap valveIndexMap(std::vector<Valve> const& valves);

Matrix adjacency(std::vector<Valve> const& valves, ValveIndexMap const& vmap);

Matrix extendPaths(Matrix const& m);

Matrix allPairsShortestPath(Matrix const& m);


struct PathScore {
    int score;
    int terminatesAt;
};

PathScore pathScore(Matrix const& apsp, std::vector<int> const& flows, std::vector<int> const& path);

std::vector<int> extractFlows(std::vector<Valve> const& valves, ValveIndexMap const& vmap);

int64_t answer1(std::vector<Valve> const& valves);

#endif
