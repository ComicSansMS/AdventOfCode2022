#include <no_space_left_on_device.hpp>

#include <string>

#include <catch.hpp>

TEST_CASE("Supply Stacks")
{
    char const sample_input[] =
        "$ cd /"         "\n"
        "$ ls"           "\n"
        "dir a"          "\n"
        "14848514 b.txt" "\n"
        "8504156 c.dat"  "\n"
        "dir d"          "\n"
        "$ cd a"         "\n"
        "$ ls"           "\n"
        "dir e"          "\n"
        "29116 f"        "\n"
        "2557 g"         "\n"
        "62596 h.lst"    "\n"
        "$ cd e"         "\n"
        "$ ls"           "\n"
        "584 i"          "\n"
        "$ cd .."        "\n"
        "$ cd .."        "\n"
        "$ cd d"         "\n"
        "$ ls"           "\n"
        "4060174 j"      "\n"
        "8033020 d.log"  "\n"
        "5626152 d.ext"  "\n"
        "7214296 k"      "\n";

    auto const filesystem = parseInput(sample_input);
    Directory const& root_dir = *filesystem.root;

    SECTION("Parse Input")
    {
        CHECK(root_dir.name == "/");
        CHECK(root_dir.path == "/");
        CHECK(root_dir.was_listed);
        REQUIRE(root_dir.directories.size() == 2);
        CHECK(root_dir.parent == nullptr);
        CHECK(root_dir.files == std::vector<File>{
            { .name = "b.txt", .size = 14848514 },
            { .name = "c.dat", .size = 8504156 },
        });
        
        Directory const& dir_a = *root_dir.directories[0];
        REQUIRE(dir_a.name == "a");
        REQUIRE(dir_a.path == "/a");
        CHECK(dir_a.was_listed);
        CHECK(dir_a.parent == &root_dir);
        CHECK(dir_a.files == std::vector<File>{
            { .name = "f", .size = 29116 },
            { .name = "g", .size = 2557 },
            { .name = "h.lst", .size = 62596 },
        });
        REQUIRE(dir_a.directories.size() == 1);
        
        Directory const& dir_a_e = *dir_a.directories[0];
        REQUIRE(dir_a_e.name == "e");
        REQUIRE(dir_a_e.path == "/a/e");
        CHECK(dir_a_e.was_listed);
        CHECK(dir_a_e.parent == &dir_a);
        CHECK(dir_a_e.files == std::vector<File>{
            { .name = "i", .size = 584 },
        });
        CHECK(dir_a_e.directories.empty());

        Directory const& dir_d = *root_dir.directories[1];
        REQUIRE(dir_d.name == "d");
        REQUIRE(dir_d.path == "/d");
        CHECK(dir_d.was_listed);
        CHECK(dir_d.parent == &root_dir);
        CHECK(dir_d.files == std::vector<File>{
            { .name = "j", .size = 4060174 },
            { .name = "d.log", .size = 8033020 },
            { .name = "d.ext", .size = 5626152 },
            { .name = "k", .size = 7214296 },
        });
        CHECK(dir_d.directories.empty());

        SECTION("Directory Total Size")
        {
            CHECK(dir_a_e.total_size == 584);
            CHECK(dir_a.total_size == 94853);
            CHECK(dir_d.total_size == 24933642);
            CHECK(root_dir.total_size == 48381165);
        }
    }

    SECTION("Answer 1")
    {
        CHECK(answer1(filesystem) == 95437);
    }

    SECTION("Answer 2")
    {
        CHECK(answer2(filesystem) == 24933642);
    }
}
