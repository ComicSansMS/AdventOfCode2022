#include <beacon_exclusion_zone.hpp>

#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include <chrono>

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

    auto const beacon_pairs = parseInput(*input);
    fmt::print("First result is {}\n", answer1(beacon_pairs, 2000000));
    fmt::print("Second result is {}\n", answer2(beacon_pairs, Range{ .start = 0, .stop = 4000000 }));

    return 0;
}
