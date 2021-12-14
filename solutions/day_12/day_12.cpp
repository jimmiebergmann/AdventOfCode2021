#include "utility.hpp"
#include <memory>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

bool is_upper(const std::string& value)
{
    for (auto& c : value)
    {
        if (!std::isupper(c))
        {
            return false;
        }
    }
    return true;
}

struct Cave;
using CavePointer = std::shared_ptr<Cave>;

struct Cave
{
    std::string name;
    bool is_big_cave;
    std::vector<CavePointer> connections;

    bool is_connected(const CavePointer& other_cave)
    {
        return std::find(connections.begin(), connections.end(), other_cave) != connections.end();
    }
};

struct CaveSystem
{
    CavePointer start_cave;
    CavePointer end_cave;
    std::map<std::string, CavePointer> cave_map;

    CavePointer find_or_create_cave(std::string& name)
    {
        auto cave = find_cave(name);
        if (!cave)
        {
            cave = std::make_shared<Cave>();
            cave->name = name;
            cave->is_big_cave = is_upper(name);
            cave_map.insert({ name, cave });
        }
        return cave;
    }

    CavePointer find_cave(const std::string& name)
    {
	    if(auto it = cave_map.find(name); it != cave_map.end())
	    {
            return it->second;
	    }
        return nullptr;
    }
};

CaveSystem parse_input(const std::vector<std::string>& values)
{
    CaveSystem cave_system;

    for(const auto& value : values)
    {
    	auto pos = value.find_first_of('-');
        auto lhs = value.substr(0, pos);
        auto rhs = value.substr(pos + 1);

        auto lhs_cave = cave_system.find_or_create_cave(lhs);;
        auto rhs_cave = cave_system.find_or_create_cave(rhs);

        if (!lhs_cave->is_connected(rhs_cave))
        {
            lhs_cave->connections.push_back(rhs_cave);
        }

        if (!rhs_cave->is_connected(lhs_cave))
        {
            rhs_cave->connections.push_back(lhs_cave);
        }
    }

    if (auto it = cave_system.cave_map.find("start"); it == cave_system.cave_map.end())
    {
        throw aoc::puzzle_exception("Failed to find start cave.");
    }
    else
    {
        cave_system.start_cave = it->second;
    }

    if (auto it = cave_system.cave_map.find("end"); it == cave_system.cave_map.end())
    {
        throw aoc::puzzle_exception("Failed to find end cave.");
    }
    else
    {
        cave_system.end_cave = it->second;
    }

    return cave_system;
}

using SmallCaveVisitCallback = std::function<bool(const std::pair<CavePointer, size_t>&, size_t)>;

size_t find_distinct_paths(const CaveSystem& cave_system, const SmallCaveVisitCallback& small_cave_visit_callback)
{
    using TraverseFunction = std::function<void(const CavePointer&, std::vector<CavePointer>, std::map<CavePointer, size_t>, const SmallCaveVisitCallback&)>;

    size_t distinct_paths = 0;
    TraverseFunction traverse;
    traverse = [&](const CavePointer& current_cave, std::vector<CavePointer> visted_caves, std::map<CavePointer, size_t> visited_small_caves, const SmallCaveVisitCallback& small_cave_check_func)
    {
        auto get_max_visited_small_cave = [&]() -> size_t
        {
            size_t max = 0;
            for (const auto& visited_small_cave : visited_small_caves)
            {
                if (visited_small_cave.second > max)
                {
                    max = visited_small_cave.second;
                }
            }

            return max;
        };

        if (!current_cave->is_big_cave)
        {
            if (auto it = visited_small_caves.find(current_cave); it != visited_small_caves.end())
            {
                if (!small_cave_check_func(*it, get_max_visited_small_cave()))
                {
                    return;
                }
                ++it->second;
            }
            else
            {
                if (!small_cave_check_func({ current_cave, 0 }, get_max_visited_small_cave()))
	            {
	                return;
	            }
	            visited_small_caves.insert({ current_cave, 1 });
			}
        }
        
        visted_caves.push_back(current_cave);

        if(current_cave == cave_system.end_cave)
        {
            ++distinct_paths;
            return;
        }

        for(const auto& connection : current_cave->connections)
        {
            traverse(connection, visted_caves, visited_small_caves, small_cave_check_func);
        }
    };

    traverse(cave_system.start_cave, { }, { }, small_cave_visit_callback);

    return distinct_paths;
}

size_t solve_part_1(const std::vector<std::string>& values)
{
    auto cave_system = parse_input(values);
    return find_distinct_paths(cave_system, [](const std::pair<CavePointer, size_t>& small_cave, size_t)
    {
	    return small_cave.second < 1;
    });
}


size_t solve_part_2(const std::vector<std::string>& values)
{
    auto cave_system = parse_input(values);
    return find_distinct_paths(cave_system, [](const std::pair<CavePointer, size_t>& small_cave, size_t max_small_cave_visits)
    {
        return (small_cave.second < 1 || max_small_cave_visits < 2) && (small_cave.first->name != "start" || small_cave.second < 1);
    });
}

int main() {
    aoc::solve_puzzles(
        aoc::read_input<12, std::string>(),
        solve_part_1,
        solve_part_2
    );
}