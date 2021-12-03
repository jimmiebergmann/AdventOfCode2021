#include "utility.hpp"

int32_t count_one_bits(const std::vector<std::string>& values, int32_t column_index)
{
    int32_t count = 0;

    for (const auto& value : values)
    {
        auto bit = static_cast<int32_t>(value[column_index] - '0');
        if (bit == 1)
        {
            ++count;
        }
    }

    return count;
}

int32_t solve_part_1(const std::vector<std::string>& values)
{
    int32_t gamma = 0;
    int32_t epsilon = 0;

    const size_t bit_count = values[0].size();
    for (int32_t i = 0; i < bit_count; i++)
    {
        const auto one_bits = count_one_bits(values, i);
        const auto zero_bits = static_cast<int32_t>(values.size()) - one_bits;

        gamma = (gamma << 1);
        epsilon = (epsilon << 1);

        if (one_bits > zero_bits)
        {
            gamma |= 1;
        }
        else
        {
            epsilon |= 1;
        }
    }

    return gamma * epsilon;
}


int32_t solve_part_2(const std::vector<std::string>& values)
{
    auto solve = [&](const char one_token, const char zero_token) -> std::string
    {
        auto values_left = values;

        const size_t bit_count = values[0].size();
        for (int32_t i = 0; i < bit_count && values_left.size() > 1; i++)
        {
            const auto one_bits = count_one_bits(values_left, i);
            const auto zero_bits = static_cast<int32_t>(values_left.size()) - one_bits;

            const auto keep_token = one_bits >= zero_bits ? one_token : zero_token;

            values_left.erase(
                std::remove_if(values_left.begin(), values_left.end(), [&](auto value) { return value[i] != keep_token; }),
                values_left.end());
        }

        if (values_left.size() != 1)
        {
            throw aoc::puzzle_exception("Failed to find value.");
        }

        return values_left[0];
    };

    auto to_number = [](std::string value) -> int32_t
    {
        int32_t result = 0;
        for (int32_t i = 0; i < value.size(); i++)
        {
            result = (result << 1);
            if (value[i] == '1')
            {
                result |= 1;
            }
        }

        return result;
    };

    const auto oxygen = to_number(solve('1', '0'));
    const auto co2 = to_number(solve('0', '1'));

    return oxygen * co2;
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<3, std::string>(true),
        solve_part_1,
        solve_part_2
    );
}