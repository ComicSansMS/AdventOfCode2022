#include <not_enough_minerals.hpp>

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

#ifdef RUNS_IN_CI
    fmt::print("Day 19 is too slow for CI.\n");
#else
    auto const blueprints = parseInput(*input);
    fmt::print("First result is {}\n", answer1(blueprints));
    int const i1 = determineMostGeodes(blueprints[0], 32);
    int const i2 = determineMostGeodes(blueprints[1], 32);
    int const i3 = determineMostGeodes(blueprints[2], 32);
    fmt::print("Second result is {}\n", i1 * i2 * i3);
#endif

    return 0;
}
