#ifndef AOC_UTILITY_UTILITY_HPP
#define AOC_UTILITY_UTILITY_HPP

#include <exception>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <type_traits>
#include <charconv>

namespace aoc
{

    struct puzzle_exception : public std::exception
    {
        puzzle_exception(const std::string& message) :
            std::exception(message.c_str())
        {}
    };


    template<typename TDataType>
    struct input_data
    {
        std::vector<TDataType> data;
        size_t day;
    };


    template<size_t VDay, typename TDataType>
    input_data<TDataType> read_input(const bool skip_empty_lines = false)
    {
        const std::string filename = "../inputs/day_" + std::to_string(VDay) + "_input.txt";
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "read_input: Failed to open input file: " << filename << "\n";
            return {};
        }

        std::vector<TDataType> data;

        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            if (line.empty() && skip_empty_lines)
            {
                continue;
            }

            if constexpr (std::is_same_v<TDataType, std::string> == true)
            {
                data.push_back(line);
            }
            else
            {
                TDataType value{};
                auto result = std::from_chars(line.data(), line.data() + line.size(), value);
                if (result.ec != std::errc())
                {
                    std::cerr << "read_input: Failed to convert input value: " << line << "\n";
                    return {};
                }

                data.push_back(value);
            }
        }

        return { std::move(data), VDay};
    }


    template<typename TInputDataType, typename ... TSolvers>
    void solve_puzzles(const input_data<TInputDataType>& input, TSolvers ... solvers)
    {
        size_t part = 0;

        try
        {
            if (input.data.empty())
            {
                throw puzzle_exception("Input data is empty.");
            }

            std::cout << "Day " << input.day << ":\n";
            (solve_puzzle(input, ++part, solvers), ...);
        }
        catch (puzzle_exception& e)
        {
            std::cerr << "Failed to solve part " << part << ": " << e.what() << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << "Internal error on " << part << ": " << e.what() << std::endl;
        }
    }

    template<typename TReturnType, typename TDataType>
    void solve_puzzle(const input_data<TDataType>& input, const size_t part, TReturnType(*solver)(const std::vector<TDataType>&))
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = solver(input.data);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Part " << part << " result: " << result << "    time: " << duration.count() << " us" << "\n";
    }
}

#endif