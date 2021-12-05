#include "utility.hpp"
#include <array>
#include <sstream>

struct Number
{
    int32_t value;
    bool marked;
};

struct Board
{
    std::array<Number, 25> numbers;
};

struct Game
{
    std::vector<int32_t> draw_numbers;
    std::vector<Board> boards;
};


std::vector<int32_t> parse_draw_numbers(const std::string& line)
{
    std::vector<int32_t> numbers = {};
    int32_t value = 0;
    char dummy = 0;

    std::stringstream ss(line);
    while (!ss.eof())
    {
        ss >> value >> dummy;
        numbers.push_back(value);
    }
    
    return numbers;
}

Board parse_board(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end)
{
    Board board{};

    int32_t value = 0;
    size_t i = 0;
    for (auto it = begin; it != end; it++)
    {
        std::stringstream ss(*it);

        for (size_t j = 0; j < 5; j++)
        {
            ss >> board.numbers[i++].value;
        }  
    }

    return board;
}

Game parse_game(const std::vector<std::string>& values)
{
    auto draw_numbers = parse_draw_numbers(values.front());

    std::vector<Board> boards;
   
    for(auto it = values.begin() + 1; it != values.end(); )
    {
        boards.push_back(parse_board(it, it + 5));
        it += 5;
    }

    return { std::move(draw_numbers), std::move(boards) };
}

bool is_winner(Board& board, int32_t index)
{
    const auto x = index % 5;
    const auto y = index / 5;

    auto check_hor = [&]()
    {
        for (int32_t i = 0; i < 5; i++)
        {
            const auto pos = (y * 5) + i;
            if (board.numbers[pos].marked == false)
            {
                return false;
            }
        }

        return true;
    };

    auto check_vert = [&]()
    {
        for (int32_t i = 0; i < 5; i++)
        {
            const auto pos = x + (i * 5);
            if (board.numbers[pos].marked == false)
            {
                return false;
            }
        }

        return true;
    };

    return check_hor() || check_vert();
}

bool set_and_check_board_number(Board& board, int32_t number)
{
    for (int32_t i = 0; i < board.numbers.size(); i++)
    {
        auto& board_number = board.numbers[i];
        if (board_number.value == number)
        {
            board_number.marked = true;
            if (is_winner(board, i))
            {
                return true;
            }

        }
    }

    return false;
}

std::pair<Board*, int32_t> find_first_winner(Game& game)
{
    for (auto& draw_number : game.draw_numbers)
    {
        for (auto& board : game.boards)
        {
            if (set_and_check_board_number(board, draw_number))
            {
                return { &board, draw_number };
            }
        }
    }

    return { nullptr, 0 };
}

std::pair<Board*, int32_t> find_last_winner(Game& game)
{
    for (auto& draw_number : game.draw_numbers)
    {
        for (auto it = game.boards.begin(); it != game.boards.end();)
        {
            auto& board = *it;
            if (set_and_check_board_number(board, draw_number))
            {
                if (game.boards.size() == 1)
                {
                    return { &board, draw_number };
                }

                it = game.boards.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    return { nullptr, 0 };
}

int32_t sum_unmarked(const Board& board)
{
    int32_t sum = 0;
    for (size_t i = 0; i < board.numbers.size(); i++)
    {
        if (!board.numbers[i].marked)
        {
            sum += board.numbers[i].value;
        }
    }
    return sum;
}

int32_t solve_part_1(const std::vector<std::string>& values)
{
    auto game = parse_game(values);
    auto[winning_board, winning_number] = find_first_winner(game);
    if (winning_board == nullptr)
    {
        throw aoc::puzzle_exception("Failed to find winning board.");
    }

    int32_t sum = sum_unmarked(*winning_board);

    return sum * winning_number;
}

int32_t solve_part_2(const std::vector<std::string>& values)
{
    auto game = parse_game(values);
    auto [winning_board, winning_number] = find_last_winner(game);
    if (winning_board == nullptr)
    {
        throw aoc::puzzle_exception("Failed to find winning board.");
    }

    int32_t sum = sum_unmarked(*winning_board);

    return sum * winning_number;
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<4, std::string>(true),
        solve_part_1,
        solve_part_2
    );
}