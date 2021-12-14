#include "utility.hpp"
#include <array>

struct Octopus
{
    int32_t energy;
};

struct Vector
{
    int32_t x;
    int32_t y;

    Vector operator +(const Vector& rhs) const
    {
        return Vector{ x + rhs.x, y + rhs.y };
    }
};

using OctopusDataRow = std::vector<Octopus>;
using OctopusData = std::vector<OctopusDataRow>;

struct OctopusMap
{
    Vector size;
    OctopusData data;
};


OctopusMap parse_input(const std::vector<std::string>& values)
{
    if(values.empty())
    {
        throw aoc::puzzle_exception("Input is empty.");
    }

    const auto dimensions = Vector{
        static_cast<int32_t>(values.front().size()),
		static_cast<int32_t>(values.size())
    };

    OctopusMap map{ dimensions, {} };
    map.data.reserve(values.size());

    for(const auto& value : values)
    {
        auto& row = *map.data.insert(map.data.end(), OctopusDataRow{});
        for(const auto number : value)
        {
            row.push_back(Octopus{ static_cast<int32_t>(number - '0') });
        }
    }

    return map;
}

bool is_inside_map(const OctopusMap& map, Vector point)
{
    return point.x >= 0 && point.y >= 0 && point.x < map.size.x && point.y < map.size.y;
}

size_t simulate_step(OctopusMap& map)
{
    static const std::array adjacent_offsets = {
        Vector{ -1, -1 }, Vector{ 0, -1 }, Vector{ 1, -1 },
        Vector{ -1, 0 }, Vector{ 1, 0 },
        Vector{ -1, 1 }, Vector{ 0, 1 }, Vector{ 1, 1 }
    };

    std::vector<std::vector<bool>> flash_map(map.size.y);
    for(auto& flash_map_row : flash_map)
    {
        flash_map_row.resize(map.size.x, false);
    }

    size_t total_flash_count = 0;
    std::vector<Vector> flashes;

    for(int32_t y = 0; y < map.size.y; y++)
    {
        for (int32_t x = 0; x < map.size.x; x++)
        {
            auto& octopus = map.data[y][x];
            if(++octopus.energy > 9)
            {
                ++total_flash_count;
                octopus.energy = 0;
                flashes.push_back(Vector{ x, y });
            }
        }
    }

    while(!flashes.empty())
    {
        std::vector<Vector> new_flashes;

        for(const auto& flash : flashes)
        {
            flash_map[flash.y][flash.x] = true;
            map.data[flash.y][flash.x].energy = 0;

            for(const auto adjacent_offset : adjacent_offsets)
            {
                auto offset_position = flash + adjacent_offset;
                if(!is_inside_map(map, offset_position))
                {
                    continue;
                }

                if(flash_map[offset_position.y][offset_position.x])
                {
                    continue;
                }

                auto& adjacent_octopus = map.data[offset_position.y][offset_position.x];
                if (++adjacent_octopus.energy > 9)
                {
                    ++total_flash_count;
                    adjacent_octopus.energy = 0;
                    new_flashes.push_back(Vector{ offset_position.x, offset_position.y });
                }
            }
        }

        flashes = new_flashes;
    }

    return total_flash_count;
}

size_t solve_part_1(const std::vector<std::string>& values)
{
    auto map = parse_input(values);

    size_t total_flash_count = 0;
    for(size_t i = 0; i < 100; i++)
    {
        total_flash_count += simulate_step(map);
    }

    return total_flash_count;
}

int32_t count_map_flashes(const OctopusMap& map)
{
    int32_t count = 0;
    for (int32_t y = 0; y < map.size.y; y++)
    {
        for (int32_t x = 0; x < map.size.x; x++)
        {
            if(map.data[y][x].energy == 0)
            {
                ++count;
            }
        }
    }
    return count;
}

size_t solve_part_2(const std::vector<std::string>& values)
{
    auto map = parse_input(values);

    for (size_t i = 0; ; i++)
    {
        simulate_step(map);

        if(count_map_flashes(map) == (map.size.x * map.size.y))
        {
            return i + 1;
        }
    }

    return 0;
}

int main() {
    aoc::solve_puzzles(
        aoc::read_input<11, std::string>(),
        solve_part_1,
        solve_part_2
    );
}