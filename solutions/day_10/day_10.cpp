#include "utility.hpp"
#include <variant>
#include <type_traits>

struct SyntaxValidationComplete {};
struct SyntaxValidationIncomplete
{
    std::vector<char> token_stack;
};
struct SyntaxValidationCorrupt
{
    char token;
};

using SyntaxValidationResult = std::variant<
    SyntaxValidationComplete,
    SyntaxValidationIncomplete,
    SyntaxValidationCorrupt>;

SyntaxValidationResult validate_syntax(const std::string& sequence)
{
    std::vector<char> token_stack;
    for(const auto token : sequence)
    {
        switch (token)
        {
        case '(': token_stack.push_back(')'); break;
        case '[': token_stack.push_back(']'); break;
        case '{': token_stack.push_back('}'); break;
        case '<': token_stack.push_back('>'); break;
        case ')': 
        case ']': 
        case '}': 
        case '>':
        {
            if(token_stack.back() != token)
            {
                return SyntaxValidationCorrupt{ token };
            }
            token_stack.pop_back();
        } break;
        default: break;
        }
    }

    if(!token_stack.empty())
    {
        return SyntaxValidationIncomplete{ std::move(token_stack) };
    }

    return SyntaxValidationComplete{ };
}

size_t get_illegal_token_score(const char token)
{
    switch(token)
    {
	    case ')': return 3;
	    case ']': return 57;
	    case '}': return 1197;
	    case '>': return 25137;
	    default: break;
    }
    return 0;
}

size_t get_autocomplete_token_score(const char token)
{
    switch (token)
    {
	    case ')': return 1;
	    case ']': return 2;
	    case '}': return 3;
	    case '>': return 4;
	    default: break;
    }
    return 0;
}

size_t solve_part_1(const std::vector<std::string>& values)
{
    size_t total_illegal_score = 0;
    for (const auto& value : values)
    {
        const auto validation_result = validate_syntax(value);
        std::visit([&](const auto& result)
            {
                if constexpr (std::is_same_v<decltype(result), const SyntaxValidationCorrupt&> == true)
                {
                    total_illegal_score += get_illegal_token_score(result.token);
                }
            }, validation_result);
    }
    return total_illegal_score;
}

size_t solve_part_2(const std::vector<std::string>& values)
{
    std::vector<size_t> autocomplete_scores;
    for (const auto& value : values)
    {
        const auto validation_result = validate_syntax(value);
        std::visit([&](const auto& result)
            {
                if constexpr (std::is_same_v<decltype(result), const SyntaxValidationIncomplete&> == true)
                {
                    size_t autocomplete_score = 0;
                    for(auto it = result.token_stack.rbegin(); it != result.token_stack.rend(); it++)
                    {
                        const auto token = *it;
                        autocomplete_score = (autocomplete_score * 5) + get_autocomplete_token_score(token);
                    }

                    autocomplete_scores.push_back(autocomplete_score);
                }
            }, validation_result);
    }

    std::sort(autocomplete_scores.begin(), autocomplete_scores.end());

    return autocomplete_scores[autocomplete_scores.size() / 2];
}

int main() {
    aoc::solve_puzzles(
        aoc::read_input<10, std::string>(),
        solve_part_1,
        solve_part_2
    );
}