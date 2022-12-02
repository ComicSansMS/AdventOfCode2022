#ifndef ADVENT_OF_CODE_02_ROCK_PAPER_SCISSORS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_02_ROCK_PAPER_SCISSORS_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

enum class Play {
    Rock, Paper, Scissors
};

enum class Second {
    X, Y, Z
};

struct Turn {
    Play opponent;
    Second second;
    friend bool operator==(Turn const&, Turn const&) = default;
};

std::vector<Turn> parseInput(std::string_view input);

struct Round {
    Play opponent;
    Play me;
    friend bool operator==(Round const&, Round const&) = default;
};

std::vector<Round> decodeTurn(std::vector<Turn> const& turns);

int64_t shapeScore(Round r);

int64_t winningScore(Round r);

int64_t calculateScore(Round r);

int64_t totalScore(std::vector<Round> const& r);

std::vector<Round> decodeTurnProper(std::vector<Turn> const& turns);

#endif
