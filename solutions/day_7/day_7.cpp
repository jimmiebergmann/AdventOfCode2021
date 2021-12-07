#include "utility.hpp"
#include <sstream>
#include <limits>
#include <functional>

struct CrabGroup
{
    int32_t position;
    int32_t crab_count;
};

using CrabGroupList = std::vector<CrabGroup>;

void add_crab_to_group(CrabGroupList& crab_group_list, int32_t position, int32_t count = 1)
{
    auto it = std::find_if(crab_group_list.begin(), crab_group_list.end(), [&](const CrabGroup& crab_group)
        {
            return crab_group.position == position;
        });

    if(it == crab_group_list.end())
    {
        crab_group_list.push_back(CrabGroup{ position, count });
    }
    else
    {
        it->crab_count += count;
    }
}

CrabGroupList parse_input(const std::vector<std::string>& values)
{
    if(values.size() != 1)
    {
        throw aoc::puzzle_exception("Expecting 1 line of input data.");
    }

    CrabGroupList crab_group_list;

    std::stringstream ss(values[0]);
    while(!ss.eof())
    {
        int32_t position = 0;
        char separator = 0;
        ss >> position >> separator;
        add_crab_to_group(crab_group_list, position);
    }

    std::sort(crab_group_list.begin(), crab_group_list.end(), [](const CrabGroup& lhs, const CrabGroup& rhs)
        {
            return lhs.position < rhs.position;
        });

    return crab_group_list;
}

int32_t find_cheapest_move_step(const CrabGroupList& crab_group_list, int32_t position, const auto& step_function)
{
    int32_t total_fuel_usage = 0;

    for(const auto crab_group : crab_group_list)
    {
        const auto fuel_usage = step_function(std::abs(crab_group.position - position));
        total_fuel_usage += fuel_usage * crab_group.crab_count;
    }

    return total_fuel_usage;
}

int32_t find_cheapest_move(const CrabGroupList& crab_group_list, const auto& step_function)
{
    const auto min_position = crab_group_list.front().position;
    const auto max_position = crab_group_list.back().position;

    auto min_fuel_usage = std::numeric_limits<int32_t>::max();

    for(auto i = min_position; i <= max_position; i++)
    {
        const auto step_result = find_cheapest_move_step(crab_group_list, i, step_function);
        if(step_result < min_fuel_usage)
        {
            min_fuel_usage = step_result;
        }
    }

    return min_fuel_usage;
}

int32_t solve_part_1(const std::vector<std::string>& values)
{
    auto crab_group_list = parse_input(values);
    return find_cheapest_move(crab_group_list, [](auto steps) { return steps; });
}

int32_t solve_part_2(const std::vector<std::string>& values)
{
    auto crab_group_list = parse_input(values);
    return find_cheapest_move(crab_group_list, [](auto steps) { return (steps * (steps + 1)) / 2; });
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<7, std::string>(),
        solve_part_1,
        solve_part_2
    );
}