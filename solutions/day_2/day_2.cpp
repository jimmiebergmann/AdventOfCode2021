#include "utility.hpp"

int32_t solve_part_1(const std::vector<std::string>& values)
{
    int32_t x = 0, y = 0;
    for (size_t i = 0; i < values.size(); i += 2)
    {
        const auto& command = values[i];
        const auto value = atoi(values[i + 1].c_str());

        switch (command[0])
        {
        case 'f': x += value; break;
        case 'd': y += value; break;
        case 'u': y -= value; break;
        default: throw aoc::puzzle_exception("Invalid command:" + command);
        }
    }

    return x * y;
}

int32_t solve_part_2(const std::vector<std::string>& values)
{
    int32_t x = 0, y = 0, aim = 0;
    for (size_t i = 0; i < values.size(); i += 2)
    {
        const auto& command = values[i];
        const auto value = atoi(values[i + 1].c_str());

        switch (command[0])
        {
        case 'f':
        {
            x += value;
            y += aim * value;
        } break;
        case 'd': aim += value; break;
        case 'u': aim -= value; break;
        default: throw aoc::puzzle_exception("Invalid command:" + command);
        }
    }

    return x * y;
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<2, std::string>(true, true),
        solve_part_1,
        solve_part_2
    );
}