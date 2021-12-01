#include "utility.hpp"

size_t solve_part_1(const std::vector<int64_t>& values)
{
    size_t result = 0;
    for (size_t i = 1; i < values.size(); i++)
    {
        if (values[i - 1] < values[i])
        {
            ++result;
        }
    }

    return result;
}

int64_t solve_part_2(const std::vector<int64_t>& values)
{
    size_t result = 0;
    for (size_t i = 1; i < values.size() - 2; i++)
    {
        const auto prevSum = values[i - 1] + values[i] + values[i + 1];
        const auto curSum = values[i] + values[i + 1] + values[i + 2];

        if (prevSum < curSum)
        {
            ++result;
        }
    }

    return result;
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<1, int64_t>(true),
        solve_part_1,
        solve_part_2
    );
}