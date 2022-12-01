#include <calorie_counting.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/partial_sort.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <cassert>
#include <optional>
#include <string>

std::vector<Elf> parseInput(std::string_view input)
{
    std::vector<Elf> ret;
    ret.emplace_back();
    for (auto const sv : input | ranges::views::split('\n')) {
        Elf& current_elf = ret.back();
        if (sv.empty()) {
            assert(!current_elf.calories.empty());
            ret.emplace_back();
        } else {
            current_elf.calories.push_back(std::stoi(ranges::to<std::string>(sv)));
        }
    }
    return ret;
}

std::int64_t totalCalories(Elf const& e)
{
    return ranges::accumulate(e.calories, 0);
}

std::int64_t maxCalorieElf(std::vector<Elf> const& v)
{
    auto rng = v | ranges::views::transform(totalCalories);
    return ranges::distance(ranges::begin(rng), ranges::max_element(rng));
}

std::int64_t answer1(std::vector<Elf> const& v)
{
    std::int64_t const max_index = maxCalorieElf(v);
    return totalCalories(v[max_index]);
}

std::int64_t answer2(std::vector<Elf> const& v)
{
    std::vector<std::int64_t> av = v | ranges::views::transform(totalCalories) | ranges::to<std::vector>;
    ranges::partial_sort(av, ranges::begin(av) + 3, ranges::greater{});
    return ranges::accumulate(ranges::subrange(ranges::begin(av), ranges::begin(av) + 3), 0);
}
