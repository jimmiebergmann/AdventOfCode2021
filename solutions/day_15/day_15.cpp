#include "utility.hpp"
#include "array2.hpp"
#include <array>

using namespace aoc;

Array2ui8 parse_input(const std::vector<std::string>& values)
{
    auto new_array = aoc::make_array2<uint8_t>(Vector2s{values.front().size(), values.size()});

    for(size_t y = 0; y < values.size(); y++)
    {
        for (size_t x = 0; x < values[y].size(); x++)
        {
            new_array.data[y][x] = static_cast<uint8_t>(values[y][x] - '0');
        }
    }

    return new_array;
}

int32_t find_lowest_risk(const Array2ui8& map, const Vector2i32 start, const Vector2i32 end)
{
    struct Node
    {
        bool closed = false;
        bool open = false;
        int32_t score = 0;
        Vector2i32 position;
    };

    static const std::array offsets = {
        Vector2i32{-1, 0 }, Vector2i32{1, 0 }, Vector2i32{0, -1 }, Vector2i32{0, 1 }
    };

    auto process_map = make_array2<Node>(map.size);
    std::vector<Node*> open_nodes;

    auto insert_open_node = [&process_map , &open_nodes](Vector2i32 position, int32_t score)
    {
        auto* node = &process_map.data[position.y][position.x];
        node->position = position;
        node->score = score;
        node->open = true;
        open_nodes.insert(
	        std::lower_bound(open_nodes.begin(), open_nodes.end(), node, [](auto& lhs, auto& rhs)
	        {
				return lhs->score > rhs->score;
	        }), node
        );
    };

    auto is_closed_node = [&process_map](Vector2i32 position)
    {
        return process_map.data[position.y][position.x].closed;
    };

    auto is_open_node = [&process_map](Vector2i32 position)
    {
        return process_map.data[position.y][position.x].open;
    };

    insert_open_node(start, 0);

    while(!open_nodes.empty())
    {
        auto& next_node = *open_nodes.back();
        open_nodes.erase(open_nodes.begin() + open_nodes.size() - 1);
        next_node.closed = true;

        for(auto& offset : offsets)
        {
            auto neighbour_pos = next_node.position + offset;

            if(map.is_outside(neighbour_pos) || is_closed_node(neighbour_pos) || is_open_node(neighbour_pos))
            {
                continue;
            }
            
            auto neighbour_score = next_node.score + (map.data[neighbour_pos.y][neighbour_pos.x]);
            insert_open_node(neighbour_pos, neighbour_score);

            if (neighbour_pos == end)
            {
                return process_map.data[end.y][end.x].score;
            }
        }
    }

    throw aoc::puzzle_exception("Failed to find end!");    
}

size_t solve_part_1(const std::vector<std::string>& values)
{
    const auto map = parse_input(values);
    const auto start = Vector2i32{ 0, 0 };
    const auto end = Vector2i32{ static_cast<int32_t>(map.size.x - 1), static_cast<int32_t>(map.size.y - 1) };
    return find_lowest_risk(map, start, end);
}

size_t solve_part_2(const std::vector<std::string>& values)
{
    return 0;
}

int main() {
    solve_puzzles(
        aoc::read_input<15, std::string>(),
        solve_part_1,
        solve_part_2
    );
}