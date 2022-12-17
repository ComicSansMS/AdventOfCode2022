#include <proboscidea_volcanium.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Proboscidea Volcanium")
{
    char const sample_input[] =
        "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB"  "\n"
        "Valve BB has flow rate=13; tunnels lead to valves CC, AA"     "\n"
        "Valve CC has flow rate=2; tunnels lead to valves DD, BB"      "\n"
        "Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE" "\n"
        "Valve EE has flow rate=3; tunnels lead to valves FF, DD"      "\n"
        "Valve FF has flow rate=0; tunnels lead to valves EE, GG"      "\n"
        "Valve GG has flow rate=0; tunnels lead to valves FF, HH"      "\n"
        "Valve HH has flow rate=22; tunnel leads to valve GG"          "\n"
        "Valve II has flow rate=0; tunnels lead to valves AA, JJ"      "\n"
        "Valve JJ has flow rate=21; tunnel leads to valve II"          "\n";

    auto const valves = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(valves.size() == 10);
        CHECK(valves[0] == Valve{ .name = "AA", .flow_rate = 0, .connections = { "DD", "II", "BB" } });
        CHECK(valves[1] == Valve{ .name = "BB", .flow_rate = 13, .connections = { "CC", "AA" } });
        CHECK(valves[2] == Valve{ .name = "CC", .flow_rate = 2, .connections = { "DD", "BB" } });
        CHECK(valves[3] == Valve{ .name = "DD", .flow_rate = 20, .connections = { "CC", "AA", "EE" }});
        CHECK(valves[4] == Valve{ .name = "EE", .flow_rate = 3, .connections = { "FF", "DD" }});
        CHECK(valves[5] == Valve{ .name = "FF", .flow_rate = 0, .connections = { "EE", "GG" }});
        CHECK(valves[6] == Valve{ .name = "GG", .flow_rate = 0, .connections = { "FF", "HH" }});
        CHECK(valves[7] == Valve{ .name = "HH", .flow_rate = 22, .connections = { "GG" }});
        CHECK(valves[8] == Valve{ .name = "II", .flow_rate = 0, .connections = { "AA", "JJ" }});
        CHECK(valves[9] == Valve{ .name = "JJ", .flow_rate = 21, .connections = { "II" }});
    }

    auto const vmap = valveIndexMap(valves);
    auto const m = allPairsShortestPath(adjacency(valves, vmap));
    std::experimental::mdspan mds(m.m.data(), m.dimension, m.dimension);
    
    SECTION("All Pairs Shortest Paths")
    {
        REQUIRE(m.dimension == 10);
        REQUIRE(m.m.size() == 100);
        CHECK(mds(vmap.at("AA"), vmap.at("AA")) == 0);
        CHECK(mds(vmap.at("AA"), vmap.at("BB")) == 1);
        CHECK(mds(vmap.at("BB"), vmap.at("BB")) == 0);
        CHECK(mds(vmap.at("AA"), vmap.at("CC")) == 2);
        CHECK(mds(vmap.at("AA"), vmap.at("JJ")) == 2);
        CHECK(mds(vmap.at("AA"), vmap.at("DD")) == 1);
        CHECK(mds(vmap.at("AA"), vmap.at("HH")) == 5);
        CHECK(mds(vmap.at("JJ"), vmap.at("HH")) == 7);
        for (int i = 0; i < mds.extent(0); ++i) {
            for (int j = 0; j < mds.extent(1); ++j) {
                CHECK(mds(i, j) == mds(j, i));
            }
        }
    }

    SECTION("Path Score")
    {
        auto const flows = extractFlows(valves, vmap);
        CHECK(pathScore(m, flows, std::vector<int>{
                vmap.at("BB"),
                vmap.at("CC"),
        }).score == 364 + 52);
        CHECK(pathScore(m, flows, std::vector<int>{
            vmap.at("DD"),
            vmap.at("BB"),
            vmap.at("JJ"),
            vmap.at("HH"),
            vmap.at("EE"),
            vmap.at("CC"),
        }).score == 1651);
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(valves) == 1651);
    }
}
