#ifndef ADVENT_OF_CODE_07_NO_SPACE_LEFT_ON_DEVICE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_07_NO_SPACE_LEFT_ON_DEVICE_HPP_INCLUDE_GUARD

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

struct File {
    std::string name;
    std::int64_t size;

    friend bool operator==(File const&, File const&) = default;
};

struct Directory {
    std::string name;
    std::string path;
    std::vector<File> files;
    std::vector<std::unique_ptr<Directory>> directories;
    Directory* parent;
    bool was_listed;
    std::int64_t total_size;

    Directory(Directory* parent, std::string_view name);
};

struct FileSystem {
    std::unique_ptr<Directory> root;
};

FileSystem parseInput(std::string_view input);

void traverseTree(Directory const& dir, std::function<void(Directory const&)> const& visitor);

std::int64_t answer1(FileSystem const& fs);

std::int64_t answer2(FileSystem const& fs);

#endif
