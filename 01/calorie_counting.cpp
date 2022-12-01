#include <calorie_counting.hpp>

#include <algorithm>
#include <cassert>
#include <ranges>
#include <numeric>
#include <optional>
#include <string>

std::vector<Elf> parseInput(std::string_view input)
{
    std::vector<Elf> ret;
    ret.emplace_back();
    for (auto const sv : input | std::ranges::views::split('\n')) {
        Elf& current_elf = ret.back();
        if (sv.empty()) {
            assert(!current_elf.calories.empty());
            ret.emplace_back();
        } else {
            current_elf.calories.push_back(std::stoi(std::ranges::to<std::string>(sv)));
        }
    }
    return ret;
}

std::int64_t totalCalories(Elf const& e)
{
    return std::accumulate(std::ranges::begin(e.calories), std::ranges::end(e.calories), std::int64_t{ 0 });
}

std::int64_t maxCalorieElf(std::vector<Elf> const& v)
{
    auto rng = v | std::ranges::views::transform(totalCalories);
    return std::ranges::distance(std::ranges::begin(rng), std::ranges::max_element(rng));
}

std::int64_t answer1(std::vector<Elf> const& v)
{
    std::int64_t const max_index = maxCalorieElf(v);
    return totalCalories(v[max_index]);
}

std::int64_t answer2(std::vector<Elf> const& v)
{
    std::vector<std::int64_t> av = v | std::ranges::views::transform(totalCalories) | std::ranges::to<std::vector>();
    std::ranges::partial_sort(av, std::ranges::begin(av) + 3, std::ranges::greater{});
    return std::accumulate(std::ranges::begin(av), std::ranges::begin(av) + 3, std::int64_t{ 0 });
}
