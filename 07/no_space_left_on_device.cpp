#include <no_space_left_on_device.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/split.hpp>

#include <fmt/format.h>

#include <cassert>
#include <limits>
#include <regex>

Directory::Directory(Directory* parent, std::string_view name)
    :name(name), parent(parent), was_listed(false), total_size(-1)
{
    if (!parent) {
        path = name;
    } else {
        path = (parent->path.ends_with('/')) ?
            fmt::format("{}{}", parent->path, name) :
            fmt::format("{}/{}", parent->path, name);
    }
}

struct Command {
    enum {
        cd,
        ls
    } op;
    std::string arg;
};

Command parseCommand(std::string_view cmd_line)
{
    std::regex rx_line(R"(\$ (\w\w)( ([\w%.%/]+))?)");
    std::match_results<std::string_view::iterator> smatch;
    bool does_match = std::regex_match(ranges::begin(cmd_line), ranges::end(cmd_line), smatch, rx_line);
    assert(does_match);
    Command ret;
    if (smatch[1] == "cd") {
        ret.op = Command::cd;
        ret.arg = smatch[3];
    } else {
        assert(smatch[1] == "ls");
        ret.op = Command::ls;
        ret.arg = "";
    }
    return ret;
}

std::unique_ptr<Directory> parseDirectory(std::string_view line, Directory* current_dir)
{
    std::regex rx_line(R"(dir (\w+))");
    std::match_results<std::string_view::iterator> smatch;
    bool does_match = std::regex_match(ranges::begin(line), ranges::end(line), smatch, rx_line);
    assert(does_match);
    return std::make_unique<Directory>(current_dir, std::string_view(smatch[1].first, smatch[1].second));
}

File parseFile(std::string_view line)
{
    std::regex rx_line(R"((\d+) ([\w%.]+))");
    std::match_results<std::string_view::iterator> smatch;
    bool does_match = std::regex_match(ranges::begin(line), ranges::end(line), smatch, rx_line);
    assert(does_match);
    return File { .name = smatch[2], .size = std::stoi(smatch[1]) };
}

Directory* findSubdirectory(Directory const& current_dir, std::string_view name)
{
    auto const it = ranges::find_if(current_dir.directories,
                                    [name](std::unique_ptr<Directory> const& d) { return d->name == name; });
    return (it != ranges::end(current_dir.directories)) ? it->get() : nullptr;
}

Directory* executeCommand(Command const& command, Directory* current_dir, Directory* root_dir, bool& in_listing)
{
    if (command.op == Command::cd) {
        assert(!command.arg.empty());
        if (command.arg == "..") {
            assert(current_dir->parent);
            current_dir = current_dir->parent;
        } else if (command.arg == "/") {
            current_dir = root_dir;
        } else {
            assert(!command.arg.contains('.'));
            assert(!command.arg.contains('/'));
            current_dir = findSubdirectory(*current_dir, command.arg);;
            assert(current_dir);
        }
    } else {
        assert(command.op == Command::ls);
        assert(!current_dir->was_listed);
        current_dir->was_listed = true;
        in_listing = true;
    }
    return current_dir;
}

std::int64_t calculateTotalSize(Directory& dir)
{
    if (dir.total_size == -1) {
        auto const files_size = ranges::accumulate(dir.files, std::int64_t{ 0 }, ranges::plus{},
                                                   [](File const& f) -> std::int64_t { return f.size; });
        auto const subdirectories_size =
            ranges::accumulate(dir.directories, std::int64_t{ 0 }, ranges::plus{},
                               [](std::unique_ptr<Directory> const& pdir) { return calculateTotalSize(*pdir); });
        dir.total_size = files_size + subdirectories_size;
    }
    return dir.total_size;
}

FileSystem parseInput(std::string_view input)
{
    FileSystem ret;
    ret.root = std::make_unique<Directory>(nullptr, "/");
    Directory* current_dir = ret.root.get();
    bool in_listing = false;
    for (auto l : input | ranges::views::split('\n')) {
        std::string const line = l | ranges::to<std::string>;
        if (line.starts_with('$')) {
            in_listing = false;
            auto const command = parseCommand(line);
            current_dir = executeCommand(command, current_dir, ret.root.get(), in_listing);
        } else {
            assert(in_listing);
            if (line.starts_with("dir ")) {
                current_dir->directories.emplace_back(parseDirectory(line, current_dir));
            } else {
                current_dir->files.emplace_back(parseFile(line));
            }
        }
    }
    calculateTotalSize(*ret.root);
    return ret;
}

void traverseTree(Directory const& dir, std::function<void(Directory const&)> const& visitor)
{
    std::vector<Directory const*> stack;
    stack.push_back(&dir);
    while (!stack.empty()) {
        Directory const* d = stack.back();
        stack.pop_back();
        visitor(*d);
        for (auto const& subdir : d->directories) {
            stack.push_back(subdir.get());
        }
    }
}

std::int64_t answer1(FileSystem const& fs)
{
    std::int64_t ret = 0;
    traverseTree(*fs.root, [&ret](Directory const& dir) {
            auto const size = dir.total_size;
            if (size <= 100000) { ret += size; }
        });
    return ret;
}

std::int64_t answer2(FileSystem const& fs)
{
    std::int64_t const unused_space = 70000000 - fs.root->total_size;
    std::int64_t const to_free = 30000000 - unused_space;
    std::int64_t ret = std::numeric_limits<std::int64_t>::max();
    traverseTree(*fs.root, [&ret, to_free](Directory const& dir) {
            auto const size = dir.total_size;
            if (size >= to_free) { ret = std::min(ret, size); }
        });
    return ret;
}
