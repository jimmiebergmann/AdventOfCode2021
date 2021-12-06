#include "utility.hpp"
#include <sstream>

struct FishGroup
{
    int32_t cycle;
    size_t fish_count;
};

using FishGroupList = std::vector<FishGroup>;

void add_fish_to_group(FishGroupList& fish_grooup_list, int32_t cycle, size_t count = 1)
{
    if(count == 0)
    {
        return;
    }

    auto it = std::find_if(fish_grooup_list.begin(), fish_grooup_list.end(), 
        [&](FishGroup& group) {
            return group.cycle == cycle;
        });

    if (it == fish_grooup_list.end())
    {
        fish_grooup_list.push_back(FishGroup{ cycle, count });
    }
    else
    {
        it->fish_count += count;
    }
}

FishGroupList parse_input(const std::vector<std::string>& values)
{
    if (values.size() != 1)
    {
        throw aoc::puzzle_exception("Invalid input data.");
    }

    FishGroupList fish_group_list;

    std::stringstream ss(values[0]);
    while (!ss.eof())
    {
        int32_t cycle = 0;
        char separator = 0;

        ss >> cycle >> separator;
        add_fish_to_group(fish_group_list, cycle);
    }

    return fish_group_list;
}

void simulate_day(FishGroupList& fish_group_list)
{
    size_t new_fish = 0;

    for(auto& fish_group : fish_group_list)
    {
        if (fish_group.cycle == 0)
        {
            fish_group.cycle = 6;
            new_fish += fish_group.fish_count;
        }
        else
        {
            --fish_group.cycle;
        }
    }

    add_fish_to_group(fish_group_list, 8, new_fish);
}

size_t simulate(FishGroupList& fish_group_list, size_t days)
{
    for (size_t i = 0; i < days; i++)
    {
        simulate_day(fish_group_list);
    }

    size_t sum = 0;
    for(const auto& fish_group : fish_group_list)
    {
        sum += fish_group.fish_count;
    }

    return sum;
}

size_t solve_part_1(const std::vector<std::string>& values)
{
    auto fish_group_list = parse_input(values);
    return simulate(fish_group_list, 80);
}

size_t solve_part_2(const std::vector<std::string>& values)
{
    auto fish_group_list = parse_input(values);
    return simulate(fish_group_list, 256);
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<6, std::string>(),
        solve_part_1,
        solve_part_2
    );
}