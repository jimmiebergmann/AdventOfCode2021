#include "utility.hpp"
#include <array>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>

using namespace aoc;

std::vector<bool> parse_input(const std::vector<std::string>& values)
{
    static const std::array bitmask = { uint8_t{ 8 }, uint8_t{ 4 }, uint8_t{ 2 }, uint8_t{ 1 } };
    const auto& input_string = values.front();
    std::vector<bool> result(input_string.size() * 4);

    size_t value_index = 0;
    for(const auto character : input_string)
    {
        const auto hex_to_decimal = (character >= '0' && character <= '9') ? 
            static_cast<uint8_t>(character - '0') : (
            (character >= 'A' && character <= 'F') ? 
                static_cast<uint8_t>(character - 'A' + 10) : 
                uint8_t{ 0 });

        for(size_t i = 0; i < bitmask.size(); i++)
        {
	        if(hex_to_decimal & bitmask[i])
	        {
                result[value_index] = true;
	        }
            ++value_index;
        }
    }

    return result;
}

template<typename T>
T combine_bits_and_advance(std::vector<bool>::const_iterator& from_it, const size_t count)
{
    T result{ 0 };
    auto it = from_it;

    for(size_t i = 0; i < count; i++)
    {
        if(*(it++))
        {
            result |= size_t{ 1 } << (count - size_t{ 1 } - i);
        }
    }

    std::advance(from_it, count);
    return result;
}

template<typename T, typename TWord>
T combine_bits(const std::vector<TWord>& words, const size_t word_width)
{
    T result{ 0 };

    for(size_t i = 0; i < words.size(); i++)
    {
        const auto word = words[i];
        result |= word << (word_width * (words.size() - size_t{ 1 } - i));
    }

    return result;
}

using ExecutionResult = uint64_t;

ExecutionResult execute_transmission(const std::vector<bool>& transmission, std::function<void(ExecutionResult, ExecutionResult)> on_packet_header = {})
{
    using Iterator = std::vector<bool>::const_iterator;
    using PacketResult = std::pair<Iterator, ExecutionResult>;

    std::function<PacketResult(Iterator)> traverse;
    traverse = [&traverse, &on_packet_header](Iterator from_it) -> PacketResult
    {
        const auto packet_version = combine_bits_and_advance<ExecutionResult>(from_it, 3);
        const auto packet_type = combine_bits_and_advance<ExecutionResult>(from_it, 3);

        if(on_packet_header)
        {
            on_packet_header(packet_version, packet_type);
        }

        auto parse_literal_value = [](Iterator from_it) -> PacketResult
        {
            bool is_last_group = false;
            std::vector<ExecutionResult> groups;
            do
            {
                is_last_group = !*(from_it++);
                groups.push_back(combine_bits_and_advance<ExecutionResult>(from_it, 4));

            } while (!is_last_group);

            const auto literal_value = combine_bits<ExecutionResult>(groups, 4);
            return { from_it, literal_value };
        };

        auto calculate_operator_result = [](ExecutionResult packet_type, const std::vector<ExecutionResult>& packet_results) -> ExecutionResult
        {
            const size_t c = packet_results.size();
            switch(packet_type)
            {
            case 0: return std::accumulate(packet_results.begin(), packet_results.end(), ExecutionResult{ 0 });
            case 1: return std::accumulate(packet_results.begin(), packet_results.end(), ExecutionResult{ 1 }, std::multiplies());
            case 2: return *std::min_element(packet_results.begin(), packet_results.end());
            case 3: return *std::max_element(packet_results.begin(), packet_results.end());
            case 5: return packet_results[0] > packet_results[1] ? ExecutionResult{ 1 } : ExecutionResult{ 0 };
            case 6: return packet_results[0] < packet_results[1] ? ExecutionResult{ 1 } : ExecutionResult{ 0 };
            case 7: return packet_results[0] == packet_results[1] ? ExecutionResult{ 1 } : ExecutionResult{ 0 };
            default: break;
            }

            throw aoc::puzzle_exception("Invalid operator: " + std::to_string(packet_type));
        };

        auto parse_operator = [&traverse, &calculate_operator_result](Iterator from_it, const ExecutionResult packet_type) -> PacketResult
        {
            std::vector<ExecutionResult> results;

            const bool has_multiple_sub_packets = *(from_it++);
            if(!has_multiple_sub_packets)
            {
                const auto sub_packet_length = combine_bits_and_advance<ExecutionResult>(from_it, 15);
                const auto expected_end = from_it + sub_packet_length;
                while(from_it < expected_end)
                {
                    auto result = traverse(from_it);
                    results.push_back(result.second);
                    from_it = result.first;
                }
                
                if (from_it != expected_end)
                {
                    throw aoc::puzzle_exception("Operator sub-packet consumed unexpected number of bits.");
                }
            }
            else
            {
                const auto sub_packet_count = combine_bits_and_advance<ExecutionResult>(from_it, 11);
                for(uint16_t i = 0; i < sub_packet_count; i++)
                {
                    auto result = traverse(from_it);
                    results.push_back(result.second);
                    from_it = result.first;
                }
            }

            const auto operator_result = calculate_operator_result(packet_type, results);
            return { from_it, operator_result };
        };

        switch(packet_type)
        {
	        case 4: return parse_literal_value(from_it);
            default: return parse_operator(from_it, packet_type);
        }
    };

    const auto result = traverse(transmission.begin());
    for(auto it = result.first; it != transmission.end(); it++)
    {
	    if(*it)
	    {
            throw aoc::puzzle_exception("Failed to find end of transmission");
	    }
    }

    return result.second;
}

uint32_t solve_part_1(const std::vector<std::string>& values)
{
    const auto input = parse_input(values);

	uint32_t version_sum = 0;
    execute_transmission(input, [&](uint8_t packet_version, uint8_t)
    {
		version_sum += static_cast<uint32_t>(packet_version);
    });

    return version_sum;
}

ExecutionResult solve_part_2(const std::vector<std::string>& values)
{
    const auto input = parse_input(values);
    return execute_transmission(input);
}

int main() {
    solve_puzzles(
        aoc::read_input<16, std::string>(),
        solve_part_1,
        solve_part_2
    );
}