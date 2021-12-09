#include "utility.hpp"
#include <array>
#include <numeric>

struct Point
{
    int32_t x;
    int32_t y;

    Point operator +(const Point& rhs) const
    {
        return Point{ x + rhs.x, y + rhs.y };
    }

    bool operator ==(const Point& rhs) const
    {
        return  x == rhs.x && y == rhs.y;
    }
};

using HeightMapRow = std::vector<uint8_t>;
using HeightMapData = std::vector<HeightMapRow>;

struct HeightMap
{
    size_t width;
    HeightMapData data;
};

HeightMap parse_input(const std::vector<std::string>& values)
{
    HeightMap height_map{ values[0].size(), {} };

    for(const auto& value : values)
    {
        if(value.size() != height_map.width)
        {
            throw aoc::puzzle_exception("Invalid width of row. ");
        }

    	auto& row = *height_map.data.insert(height_map.data.end(), HeightMapRow{});
        row.reserve(height_map.width);

    	for(const auto number : value)
        {
            row.push_back(number - '0');
        }
    }
    return height_map;
}

bool is_inside_map(const HeightMap& height_map, const Point position)
{
    return
        position.x >= 0 && position.y >= 0 &&
        position.x < static_cast<int32_t>(height_map.width) && position.y < static_cast<int32_t>(height_map.data.size());
};

bool is_low_point(const HeightMap& height_map, const Point position)
{
    static const std::array surroundings = {
        Point{ -1, -1 }, Point{ 0, -1 }, Point{ 1, -1 },
        Point{ -1, 0 }, Point{ 1, 0 },
        Point{ -1, 1 }, Point{ 0, 1 }, Point{ 1, 1 }
    };

    for(const auto& surrounding : surroundings)
    {
        const auto surrounding_position = position + surrounding;
	    if(!is_inside_map(height_map, surrounding_position))
	    {
            continue;
	    }

        if(height_map.data[surrounding_position.y][surrounding_position.x] <= height_map.data[position.y][position.x])
        {
            return false;
        }
    }

    return true;
}

std::vector<Point> get_low_points(const HeightMap& height_map)
{
    std::vector<Point> low_points;

    for (int32_t y = 0; y < static_cast<int32_t>(height_map.data.size()); y++)
    {
        for (int32_t x = 0; x < static_cast<int32_t>(height_map.data[y].size()); x++)
        {
            const auto heigh_map_position = Point{ x, y };
            if (is_low_point(height_map, heigh_map_position))
            {
                low_points.push_back(heigh_map_position);
            }
        }
    }

    return low_points;
}

// Part 2
int32_t sum_risk_levels(const HeightMap& height_map, const std::vector<Point>& low_points)
{
    int32_t sum = 0;
    for(const auto& low_point : low_points)
    {
        sum += height_map.data[low_point.y][low_point.x] + 1;
    }
    return sum;
}


int32_t solve_part_1(const std::vector<std::string>& values)
{
    auto height_map = parse_input(values);
    auto low_points = get_low_points(height_map);
    return sum_risk_levels(height_map, low_points);
}

// Part 2
size_t get_low_point_basin(const HeightMap& height_map, const Point& low_point)
{
    static const std::array surroundings = {
        Point{ -1, 0 }, Point{ 1, 0 }, Point{ 0, -1 },Point{ 0, 1 },
    };

    std::vector<Point> processing_positions;
    std::vector<Point> visited_positions;

    auto is_visited_position = [&visited_positions](const Point& position)
    {
        return std::find_if(visited_positions.begin(), visited_positions.end(), [&](const Point& visited_position) {
                return visited_position == position;
			}) != visited_positions.end();
    };

    auto process_positions = [&]()
    {
        std::vector<Point> new_processing_positions;
        for(auto const processing_position : processing_positions)
        {
            for (const auto& surrounding : surroundings)
            {
                const auto surrounding_position = processing_position + surrounding;
                if (!is_inside_map(height_map, surrounding_position) || is_visited_position(surrounding_position))
                {
                    continue;
                }

                const auto surrounding_value = height_map.data[surrounding_position.y][surrounding_position.x];
                if (surrounding_value == 9 || surrounding_value <= height_map.data[processing_position.y][processing_position.x])
                {
                    continue;
                }

                new_processing_positions.push_back(surrounding_position);
                visited_positions.push_back(surrounding_position);
            }
        }
        processing_positions = new_processing_positions;
    };

    processing_positions.push_back(low_point);
    visited_positions.push_back(low_point);
    while(!processing_positions.empty())
    {
        process_positions();
    }

    return visited_positions.size();
}

std::vector<size_t> get_basins(const HeightMap& height_map, const std::vector<Point>& low_points)
{
    std::vector<size_t> basins;
    for(const auto& low_point : low_points)
    {
        basins.push_back(get_low_point_basin(height_map, low_point));
    }
    return basins;
}

size_t accumulate_largest_basins(const std::vector<size_t>& basins, size_t count = 3)
{
    const size_t max_count = basins.size() < count ? basins.size() : count;
    return std::accumulate(basins.begin(), basins.begin() + max_count, size_t{ 1 }, std::multiplies<>());
}

size_t solve_part_2(const std::vector<std::string>& values)
{
    const auto height_map = parse_input(values);
    const auto low_points = get_low_points(height_map);
    auto basins = get_basins(height_map, low_points);
    std::sort(basins.begin(), basins.end(), std::greater<>());
    return accumulate_largest_basins(basins);
}

int main() {
    aoc::solve_puzzles(
        aoc::read_input<9, std::string>(),
        solve_part_1,
        solve_part_2
    );
}