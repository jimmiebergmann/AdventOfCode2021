#include "utility.hpp"
#include <string>
#include <map>
#include <limits>

struct Polymer
{
    std::map<std::string, size_t> pairs;
    std::map<std::string, char> rules;
};

template<typename TK, typename TV>
void add_or_increment_map(std::map<TK, TV>& map, const TK& key, const TV count = 1)
{
    if (auto it = map.find(key); it != map.end())
    {
        it->second += count;
    }
    else
    {
        map.insert({ key, count });
    }
}

template<typename TK, typename TV>
void merge_maps(std::map<TK, TV>& to, const std::map<TK, TV>& from)
{
    for (const auto& from_pair : from)
    {
        add_or_increment_map(to, from_pair.first, from_pair.second);
    }
}

Polymer parse_input(const std::vector<std::string>& values)
{
    Polymer polymer;
    const auto pair_string = values.front();

    for (auto it = values.begin() + 2; it != values.end(); it++)
    {
        const auto& value = *it;
        const auto rule_key = value.substr(0, value.find_first_of(' '));
        const auto rule_value = value.substr(value.find_last_of(' ') + 1);
        polymer.rules.insert({ rule_key, rule_value.front() });
    }

    for (size_t i = 0; i < pair_string.size() - 1; i++)
    {
        const auto pair = std::string(1, pair_string[i]) + pair_string[i + 1];
        if (auto it = polymer.rules.find(pair); it != polymer.rules.end())
        {
            add_or_increment_map(polymer.pairs, pair);
        }
    }

    return polymer;
}

void step(Polymer& polymer)
{
    std::map<std::string, size_t> new_pairs;

    for (auto& pair : polymer.pairs)
    {
        if (auto it = polymer.rules.find(pair.first); it != polymer.rules.end())
        {
            const auto lhs_pair = std::string(1, pair.first[0]) + it->second;
            const auto rhs_pair = std::string(1, it->second) + pair.first[1];
            add_or_increment_map(new_pairs, lhs_pair, pair.second);
            add_or_increment_map(new_pairs, rhs_pair, pair.second);
            pair.second = 0;
        }
    }

    merge_maps(polymer.pairs, new_pairs);
}

size_t calc_min_max_result(const Polymer& polymer)
{
    std::map<char, size_t> elements;
    auto add_element = [&elements](const char element, const size_t count = 1)
    {
        if (auto it = elements.find(element); it != elements.end())
        {
            it->second += count;
        }
        else
        {
            elements.insert({ element, count });
        }
    };

    for (const auto& pair : polymer.pairs)
    {
        add_element(pair.first[0], pair.second);
        add_element(pair.first[1], pair.second);
    }

    size_t min = std::numeric_limits<size_t>::max();
    size_t max = 0;
    for (const auto element : elements)
    {
        if (element.second > max)
        {
            max = element.second;
        }
        if (element.second < min)
        {
            min = element.second;
        }
    }

    return (max - min + 1) / 2;
}

size_t solve_part_1(const std::vector<std::string>& values)
{
    auto polymer = parse_input(values);
    for (size_t i = 0; i < 10; i++)
    {
        step(polymer);
    }
    
    return calc_min_max_result(polymer);
}


size_t solve_part_2(const std::vector<std::string>& values)
{
    auto polymer = parse_input(values);
    for (size_t i = 0; i < 40; i++)
    {
        step(polymer);
    }

    return calc_min_max_result(polymer);
}

int main() {
    aoc::solve_puzzles(
        aoc::read_input<14, std::string>(),
        solve_part_1,
        solve_part_2
    );
}