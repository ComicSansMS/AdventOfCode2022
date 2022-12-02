#include <rock_paper_scissors.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/transform.hpp>

#include <cassert>
#include <optional>
#include <regex>
#include <string>

std::vector<Turn> parseInput(std::string_view input)
{
    std::regex rx_line(R"(([A-C]) ([X-Z]))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<Turn> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> Turn {
            Turn t;
            if (match[1] == "A") {
                t.opponent = Play::Rock;
            } else if (match[1] == "B") {
                t.opponent = Play::Paper;
            } else {
                assert(match[1] == "C");
                t.opponent = Play::Scissors;
            }
            if (match[2] == "X") {
                t.second = Second::X;
            } else if (match[2] == "Y") {
                t.second = Second::Y;
            } else {
                assert(match[2] == "Z");
                t.second = Second::Z;
            }
            return t;
        });
    return ret;
}

std::vector<Round> decodeTurn(std::vector<Turn> const& turns)
{
    return turns | ranges::views::transform([](Turn const& t) -> Round {
            Round r;
            r.opponent = t.opponent;
            if (t.second == Second::X) {
                r.me = Play::Rock;
            } else if (t.second == Second::Y) {
                r.me = Play::Paper;
            } else {
                assert(t.second == Second::Z);
                r.me = Play::Scissors;
            }
            return r;
        }) | ranges::to<std::vector>();
}

int64_t shapeScore(Round r)
{
    if (r.me == Play::Rock) {
        return 1;
    } else if (r.me == Play::Paper) {
        return 2;
    } else {
        assert(r.me == Play::Scissors);
        return 3;
    }
}

int64_t winningScore(Round r)
{
    if (r.me == r.opponent) {
        // draw
        return 3;
    }
    if (r.opponent == Play::Rock) {
        return (r.me == Play::Paper) ? 6 : 0;
    } else if (r.opponent == Play::Paper) {
        return (r.me == Play::Scissors) ? 6 : 0;
    } else {
        assert(r.opponent == Play::Scissors);
        return (r.me == Play::Rock) ? 6 : 0;
    }
}

int64_t calculateScore(Round r)
{
    return shapeScore(r) + winningScore(r);
}

int64_t totalScore(std::vector<Round> const& r)
{
    return ranges::accumulate(r, 0, ranges::plus{}, calculateScore);
}

std::vector<Round> decodeTurnProper(std::vector<Turn> const& turns)
{
    enum class Desired { Win, Loss, Draw };
    auto trans = [](Turn t) -> Round {
        Desired outcome;
        if (t.second == Second::X) {
            outcome = Desired::Loss;
        } else if (t.second == Second::Y) {
            outcome = Desired::Draw;
        } else {
            assert(t.second == Second::Z);
            outcome = Desired::Win;
        }

        Round r;
        r.opponent = t.opponent;
        if (outcome == Desired::Draw) {
            r.me = r.opponent;
        } else if (r.opponent == Play::Rock) {
            r.me = (outcome == Desired::Win) ? Play::Paper : Play::Scissors;
        } else if (r.opponent == Play::Paper) {
            r.me = (outcome == Desired::Win) ? Play::Scissors : Play::Rock;
        } else {
            assert(r.opponent == Play::Scissors);
            r.me = (outcome == Desired::Win) ? Play::Rock : Play::Paper;
        }
        return r;
    };
    return turns | ranges::views::transform(trans) | ranges::to<std::vector>;
}
