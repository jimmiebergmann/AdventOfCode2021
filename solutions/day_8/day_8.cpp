#include "utility.hpp"
#include <array>
#include <functional>
#include <sstream>

using SignalPatterns = std::array<std::string, 10>;
using OutputValues = std::array<std::string, 4>;

struct Display
{
    SignalPatterns signal_patterns;
    OutputValues output_values;
};

using Displays = std::vector<Display>;

Displays parse_input(const std::vector<std::string>& values)
{
    Displays displays;

	for(const auto& value : values)
	{
        std::stringstream ss(value);

        auto& display = *displays.insert(displays.end(), Display{});

        std::string token;

		for(size_t i = 0; i < std::tuple_size_v<SignalPatterns>; i++)
		{
            ss >> token;
            display.signal_patterns[i] = token;
		}

        ss >> token;

        for (size_t i = 0; i < std::tuple_size_v<OutputValues>; i++)
        {
            ss >> token;
            display.output_values[i] = token;
        }

        if(ss.fail())
        {
            throw aoc::puzzle_exception("Unexpected end of line.");
        }
	}

    return displays;
}

void sort_display_tokens(Displays& displays)
{
	for(auto& display : displays)
	{
        for (auto& signal_token : display.signal_patterns)
        {
            std::sort(signal_token.begin(), signal_token.end());
        }
        for (auto& output_token : display.output_values)
        {
            std::sort(output_token.begin(), output_token.end());
        }
	}
}

// Part 1
int32_t count_part_1(Displays& displays)
{
    int32_t count = 0;

    for (auto& display : displays)
    {
        for (auto& output_value : display.output_values)
        {
            if (output_value.size() == 2 || output_value.size() == 3 || output_value.size() == 4 || output_value.size() == 7)
            {
                ++count;
            }
        }
    }

    return count;
}

int32_t solve_part_1(const std::vector<std::string>& values)
{
    auto displays = parse_input(values);
    sort_display_tokens(displays);
    return count_part_1(displays);
}

// Part 2
bool token_contains(const std::string& token, const std::string& contains)
{
    for(const auto c : contains)
    {
        auto it = std::find(token.begin(), token.end(), c);
        if(it == token.end())
        {
            return false;
        }
    }

    return true;
}

std::string find_and_erase_expected_token(std::vector<std::string>& possible_tokens, std::function<bool(const std::string)> check_function)
{
    for (auto it = possible_tokens.begin(); it != possible_tokens.end(); it++)
    {
        auto posibility = *it;
        if(check_function(posibility))
        {
            possible_tokens.erase(it);
            return posibility;
        }
    }
    return "";
}

int32_t count_part_2_display(Display& display)
{
    using NumberTokens = std::array<std::string, 10>;

    std::vector<std::string> posibilities_2_3_5; // Length of 5
    std::vector<std::string> posibilities_0_6_9; // Length of 6
    NumberTokens found_tokens;

    for (auto& signal_pattern : display.signal_patterns)
    {
        switch(signal_pattern.size())
        {
	        case 2: found_tokens[1] = signal_pattern; break; // 1
	        case 3: found_tokens[7] = signal_pattern; break; // 7
	        case 4: found_tokens[4] = signal_pattern; break; // 4
	        case 7: found_tokens[8] = signal_pattern; break; // 8
	        case 5: posibilities_2_3_5.push_back(signal_pattern); break;
	        case 6: posibilities_0_6_9.push_back(signal_pattern); break;
	        default: break;
        }
    }

    found_tokens[0] = find_and_erase_expected_token(posibilities_0_6_9, [&](const auto posibility)
        {
            return token_contains(posibility, found_tokens[1]) &&
                !token_contains(posibility, found_tokens[4]) &&
                token_contains(posibility, found_tokens[7]);
        });
    found_tokens[9] = find_and_erase_expected_token(posibilities_0_6_9, [&](const auto posibility)
        {
            return !token_contains(posibility, found_tokens[0]) &&
                token_contains(posibility, found_tokens[1]) &&
                token_contains(posibility, found_tokens[4]) &&
                token_contains(posibility, found_tokens[7]);
        });
    found_tokens[6] = posibilities_0_6_9.front();
    found_tokens[5] = find_and_erase_expected_token(posibilities_2_3_5, [&](const auto posibility)
        {
            return !token_contains(posibility, found_tokens[1]) &&
                !token_contains(posibility, found_tokens[7]) &&
                token_contains(found_tokens[6], posibility);
        });
    found_tokens[3] = find_and_erase_expected_token(posibilities_2_3_5, [&](const auto posibility)
        {
            return token_contains(posibility, found_tokens[1]);
        });
    found_tokens[2] = posibilities_2_3_5.front();

    int32_t final_number = 0;
    for (const auto& output_value : display.output_values)
    {
        auto it = std::find_if(found_tokens.begin(), found_tokens.end(), [&](auto token)
            {
                return token == output_value;
            });

        final_number *= 10;
        final_number += static_cast<int32_t>(it - found_tokens.begin());
    }

    return final_number;
}

int32_t count_part_2(Displays& displays)
{
    int32_t count = 0;

    for (auto& display : displays)
    {
        count += count_part_2_display(display);
    }

    return count;
}

int32_t solve_part_2(const std::vector<std::string>& values)
{
    auto displays = parse_input(values);
    sort_display_tokens(displays);
    return count_part_2(displays);
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<8, std::string>(),
        solve_part_1,
        solve_part_2
    );
}