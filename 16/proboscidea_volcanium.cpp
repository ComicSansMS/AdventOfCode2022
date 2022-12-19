#include <proboscidea_volcanium.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <regex>


std::vector<Valve> parseInput(std::string_view input)
{
    //                            1                    2                                           3
    std::regex rx_line(R"(Valve (\w\w) has flow rate=(\d+); tunnel(?:s?) lead(?:s?) to valve(?:s?) ((?:(?:\w\w)(?:, )?)+))");
    

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    auto parseLine = [](std::match_results<std::string_view::iterator> const& match) -> Valve {
        Valve ret;
        ret.name = match[1];
        ret.flow_rate = std::stoi(match[2]);
        
        using regex_it = std::regex_iterator<std::string_view::iterator>;
        std::regex rx(R"((\w\w))");
        auto const it_begin = regex_it(match[3].first, match[3].second, rx);
        auto const it_end = regex_it();
        ret.connections = ranges::make_subrange(it_begin, it_end)
            | ranges::views::transform([](auto&& m) -> std::string { return m[1]; })
            | ranges::to<std::vector>;
        return ret;
    };

    std::vector<Valve> ret = ranges::make_subrange(it_begin, it_end) | ranges::views::transform(parseLine) | ranges::to<std::vector>;
    ranges::sort(ret, [](Valve const& lhs, Valve const& rhs) { return lhs.name < rhs.name; });
    return ret;
}

// LCOV_EXCL_START
std::string toDot(std::vector<Valve> const& valves)
{
    std::string ret;
    ret = "digraph D {" "\n";
    for (auto const& v : valves) {
        ret += fmt::format(R"(  {} [shape=box, label="{} - {}"])" "\n", v.name, v.name, v.flow_rate);
    }
    ret += "\n";
    for (auto const& v : valves) {
        std::string const& src = v.name;
        for (auto const& dst : v.connections) {
            ret += fmt::format("  {} -> {}\n", src, dst);
        }
    }
    ret += "}\n";
    return ret;
}
// LCOV_EXCL_STOP

ValveIndexMap valveIndexMap(std::vector<Valve> const& valves)
{
    ValveIndexMap ret;
    for (int i = 0; i < static_cast<int>(valves.size()); ++i) {
        ret[valves[i].name] = i;
    }
    assert(ret.at("AA") == 0);
    return ret;
}

Matrix adjacency(std::vector<Valve> const& valves, ValveIndexMap const& vmap)
{
    Matrix m;
    m.dimension = static_cast<int>(valves.size());
    m.m.resize(m.dimension * m.dimension, -1);
    std::experimental::mdspan mds(m.m.data(), m.dimension, m.dimension);
    for (auto const& v : valves) {
        for (auto const& t : v.connections) {
            mds(vmap.at(v.name), vmap.at(t)) = 1;
        }
        mds(vmap.at(v.name), vmap.at(v.name)) = 0;
    }
    return m;
}

Matrix extendPaths(Matrix const& m)
{
    Matrix r = m;
    std::experimental::mdspan sm(m.m.data(), m.dimension, m.dimension);
    std::experimental::mdspan sr(r.m.data(), r.dimension, r.dimension);
    for (int i = 0; i < m.dimension; ++i) {
        for (int j = 0; j < m.dimension; ++j) {
            sr(i, j) = -1;
            for (int k = 0; k < m.dimension; ++k) {
                auto const m_ik = sm(i, k);
                auto const m_kj = sm(k, j);
                if ((m_ik != -1) && (m_kj != -1)) {
                    int const candidate = m_ik + m_kj;
                    sr(i, j) = (sr(i, j) == -1) ? candidate : std::min(sr(i, j), candidate);
                }
            }
        }
    }
    return r;
}

Matrix allPairsShortestPath(Matrix const& m)
{
    Matrix r = m;
    for (int i = 0; i < m.dimension; ++i) {
        r = extendPaths(r);
    }
    return r;
}

PathScore pathScore_it(Matrix const& apsp, std::vector<int> const& flows,
    std::vector<int>::const_iterator it_path_begin, std::vector<int>::const_iterator it_path_end)
{
    int score = 0;
    int minutes_remain = 30;
    int current_node = 0;
    int terminates_at = 0;
    std::experimental::mdspan m(apsp.m.data(), apsp.dimension, apsp.dimension);
    for (auto const& n : ranges::subrange(it_path_begin, it_path_end)) {
        int const path_cost = m(current_node, n);
        int const valve_open = 1;
        minutes_remain -= path_cost + valve_open;
        if (minutes_remain < 0) { break; }
        score += minutes_remain * flows[n];
        current_node = n;
        ++terminates_at;
    }
    return PathScore{ .score = score, .terminatesAt = terminates_at };
}

PathScore pathScore(Matrix const& apsp, std::vector<int> const& flows, std::vector<int> const& path)
{
    return pathScore_it(apsp, flows, path.begin(), path.end());
}

std::vector<int> extractFlows(std::vector<Valve> const& valves, ValveIndexMap const& vmap)
{
    std::vector<int> flows;
    flows.resize(valves.size(), -1);
    for (auto const& v : valves) {
        flows[vmap.at(v.name)] = v.flow_rate;
    }
    return flows;
}

// Test sample is not complex enough to trigger skipping
// LCOV_EXCL_START
bool skipPermutations_it(std::vector<int>::iterator it_begin,
                         std::vector<int>::iterator it_end,
                         std::vector<int>::iterator it_skip_from)
{
    std::sort(it_skip_from + 1, it_end);
    // find the smallest element larger than the skip and swap with it
    auto const it_swap = std::find_if(it_skip_from + 1, it_end,
                                      [skip_element = *it_skip_from](int i) { return i > skip_element; });
    if (it_swap != it_end) {
        std::iter_swap(it_skip_from, it_swap);
    } else {
        // swap element is larger than succeeding elements; skip to last permutation in range
        // which has the succeeding elements ordered from biggest to smallest
        std::reverse(it_skip_from + 1, it_end);
        if (!std::next_permutation(it_begin, it_end)) {
            return false;
        }
    }
    return true;
}

bool skipPermutations(std::vector<int>& rng, int skip_from)
{
    return skipPermutations_it(rng.begin(), rng.end(), rng.begin() + skip_from);
}
// LCOV_EXCL_STOP

int64_t answer1(std::vector<Valve> const& valves)
{
    auto const vmap = valveIndexMap(valves);
    auto const apsp = allPairsShortestPath(adjacency(valves, vmap));
    std::experimental::mdspan m(apsp.m.data(), apsp.dimension, apsp.dimension);

    std::vector<int> const flows = extractFlows(valves, vmap);
    std::vector<int> nodes_with_flow;
    for (int i = 0; i < static_cast<int>(flows.size()); ++i) {
        if (flows[i] > 0) { nodes_with_flow.push_back(i); }
    }
    assert(std::is_sorted(nodes_with_flow.begin(), nodes_with_flow.end()));

    // all permutations
    int best_score = 0;
    for (;;) {
        auto const [new_score, terminates_at] = pathScore(apsp, flows, nodes_with_flow);
        best_score = std::max(best_score, new_score);
        if (terminates_at < static_cast<int>(nodes_with_flow.size()) - 1) {
            if (!skipPermutations(nodes_with_flow, terminates_at)) {                                // LCOV_EXCL_LINE
                break;                                                                              // LCOV_EXCL_LINE
            }
        } else if (!std::next_permutation(nodes_with_flow.begin(), nodes_with_flow.end())) {
            break;
        }
    }

    return best_score;
}

PathScore pathScore2(Matrix const& apsp, std::vector<int> const& flows, std::vector<int> const& path)
{
    int score_me = 0;
    int score_elephant = 0;
    int minutes_remain_me = 26;
    int minutes_remain_elephant = 26;
    int current_node_me = 0;
    int current_node_elephant = 0;
    int terminates_at = 0;
    bool my_turn = true;
    std::experimental::mdspan m(apsp.m.data(), apsp.dimension, apsp.dimension);
    int const valve_open = 1;
    for (auto const& n : path) {
        if ((my_turn || (minutes_remain_elephant <= 0)) && (minutes_remain_me > 0)) {
            // me
            int const path_cost = m(current_node_me, n);
            minutes_remain_me -= path_cost + valve_open;
            if (minutes_remain_me > 0) {
                score_me += minutes_remain_me * flows[n];
                current_node_me = n;
            }
            my_turn = false;
        } else if (minutes_remain_elephant > 0) {
            // elephant
            int const path_cost = m(current_node_elephant, n);
            minutes_remain_elephant -= path_cost + valve_open;
            if (minutes_remain_elephant > 0) {
                score_elephant += minutes_remain_elephant * flows[n];
                current_node_elephant = n;
            }
            my_turn = true;
        }
        if ((minutes_remain_me <= 0) && (minutes_remain_elephant <= 0)) { break; }
        ++terminates_at;
    }
    return PathScore{ .score = score_me + score_elephant, .terminatesAt = terminates_at };
}

int64_t answer2(std::vector<Valve> const& valves)
{
    auto const vmap = valveIndexMap(valves);
    auto const apsp = allPairsShortestPath(adjacency(valves, vmap));
    std::experimental::mdspan m(apsp.m.data(), apsp.dimension, apsp.dimension);

    std::vector<int> const flows = extractFlows(valves, vmap);
    std::vector<int> nodes_with_flow;
    for (int i = 0; i < static_cast<int>(flows.size()); ++i) {
        if (flows[i] > 0) { nodes_with_flow.push_back(i); }
    }
    assert(std::is_sorted(nodes_with_flow.begin(), nodes_with_flow.end()));

    // all permutations
    int best_score = 0;
    for (;;) {
        auto const [new_score, terminates_at] = pathScore2(apsp, flows, nodes_with_flow);
        best_score = std::max(best_score, new_score);
        if (terminates_at < static_cast<int>(nodes_with_flow.size()) - 1) {
            if (!skipPermutations(nodes_with_flow, terminates_at)) {                            // LCOV_EXCL_LINE
                break;                                                                          // LCOV_EXCL_LINE
            }
        } else if (!std::next_permutation(nodes_with_flow.begin(), nodes_with_flow.end())) {
            break;
        }
    }

    return best_score;
}

