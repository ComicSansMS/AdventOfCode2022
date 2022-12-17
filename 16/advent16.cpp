#include <proboscidea_volcanium.hpp>

#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include <chrono>

#include <algorithm>

std::optional<std::string> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        fmt::print(stderr, "Unable to open input file '{}' for reading.\n", filename);
        return std::nullopt;
    }

    std::stringstream sstr;
    sstr << fin.rdbuf();
    if(!fin) {
        fmt::print(stderr, "Unable to read input from file '{}'.\n", filename);
        return std::nullopt;
    }
    return sstr.str();
}

int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    auto const input = readInput(input_filename);

    if(!input) {
        return 1;
    }

    auto const valves = parseInput(*input);
    auto const vmap = valveIndexMap(valves);
    auto const m = allPairsShortestPath(adjacency(valves, vmap));
    fmt::print("First result is {}\n", answer1(valves));
    //fmt::print("Second result is {}\n", answer2(beacon_pairs, Range{ .start = 0, .stop = 4000000 }));

    return 0;
}
