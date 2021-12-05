#include "utility.hpp"
#include <memory>
#include <regex>

struct Vector
{
    int32_t x;
    int32_t y;
};

struct Line
{
    Vector from;
    Vector to;
};

struct ParseResult
{
    std::vector<Line> lines;
    Vector max_bounds;
};

struct Board
{
    std::unique_ptr<int32_t[]> data;
    Vector dimensions;
};

ParseResult parse_input(const std::vector<std::string>& values)
{
    ParseResult result{ {}, { 0, 0 } };
    result.lines.reserve(values.size());

    std::regex regex("([0-9]*),([0-9]*) -> ([0-9]*),([0-9]*)");
    std::smatch match;

    for(const auto& value : values)
    {
        if (!std::regex_search(value, match, regex) || match.size() != 5)
        {
            throw aoc::puzzle_exception("Failed to parse input.");
        }

        const auto line = Line{
            Vector {
                atoi(match[1].str().c_str()),
                atoi(match[2].str().c_str())
            }, Vector {
                atoi(match[3].str().c_str()),
                atoi(match[4].str().c_str())
            }
        };

		result.lines.push_back(line);

        if (line.from.x > result.max_bounds.x)
        {
            result.max_bounds.x = line.from.x;
        }
        if (line.from.y > result.max_bounds.y)
        {
            result.max_bounds.y = line.from.y;
        }
        if (line.to.x > result.max_bounds.x)
        {
            result.max_bounds.x = line.to.x;
        }
        if (line.to.y > result.max_bounds.y)
        {
            result.max_bounds.y = line.to.y;
        }
    }

    return result;
}

Board create_board(const Vector size)
{
    auto dimensions = Vector{ size.x + 1, size.y + 1 };
    auto data = std::make_unique<int32_t[]>(dimensions.x * dimensions.y);
    return Board{ std::move(data), dimensions };
}

void draw_straight_line(Board& board, const Line line)
{
    if(line.from.x != line.to.x && line.from.y != line.to.y)
    {
        return;
    }

    auto min_max_line = Line{
        Vector{std::min(line.from.x, line.to.x), std::min(line.from.y, line.to.y)},
        Vector{std::max(line.from.x, line.to.x), std::max(line.from.y, line.to.y)},
    };

    for(int32_t x = min_max_line.from.x; x <= min_max_line.to.x; x++)
    {
        for (int32_t y = min_max_line.from.y; y <= min_max_line.to.y; y++)
        {
            const auto pos = (y * board.dimensions.x) + x;

            ++board.data[pos];
        }
    }
}

void draw_straight_lines(Board& board, const std::vector<Line>& lines)
{
	for (auto& line : lines)
	{
        draw_straight_line(board, line);
	}
}

void draw_line(Board& board, const Line line)
{
    if (line.from.x != line.to.x && line.from.y != line.to.y)
    {
        auto min_max_line = Line{
            line.from.x < line.to.x ? line.from : line.to,
             line.from.x < line.to.x ? line.to : line.from
        };

        int32_t y = min_max_line.from.y;
        for (int32_t x = min_max_line.from.x; x <= min_max_line.to.x; x++)
        {
            const auto pos = (y * board.dimensions.x) + x;
            ++board.data[pos];

            if(min_max_line.from.y < min_max_line.to.y)
            {
                ++y;
            }
            else
            {
                --y;
            }
        }
    }
    else
    {
        auto min_max_line = Line{
			Vector{std::min(line.from.x, line.to.x), std::min(line.from.y, line.to.y)},
			Vector{std::max(line.from.x, line.to.x), std::max(line.from.y, line.to.y)},
        };

        for (int32_t x = min_max_line.from.x; x <= min_max_line.to.x; x++)
        {
            for (int32_t y = min_max_line.from.y; y <= min_max_line.to.y; y++)
            {
                const auto pos = (y * board.dimensions.x) + x;
                ++board.data[pos];
            }
        }
    }    
}

void draw_lines(Board& board, const std::vector<Line>& lines)
{
    for (auto& line : lines)
    {
        draw_line(board, line);
    }
}

int32_t count_overlaps(const Board& board, int32_t min_count)
{
    const int32_t board_size = board.dimensions.x * board.dimensions.y;

    int32_t count = 0;
	for(int32_t i = 0; i < board_size; i++)
    {
	    if(board.data[i] >= min_count)
	    {
            ++count;
	    }
    }

    return count;
}

int32_t solve_part_1(const std::vector<std::string>& values)
{
    auto result = parse_input(values);
    auto board = create_board(result.max_bounds);
    draw_straight_lines(board, result.lines);
    return count_overlaps(board, 2);
}

int32_t solve_part_2(const std::vector<std::string>& values)
{
    auto result = parse_input(values);
    auto board = create_board(result.max_bounds);
    draw_lines(board, result.lines);
    return count_overlaps(board, 2);
}

int main()
{
    aoc::solve_puzzles(
        aoc::read_input<5, std::string>(true),
        solve_part_1,
        solve_part_2
    );
}