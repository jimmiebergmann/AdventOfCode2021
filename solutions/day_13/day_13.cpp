#include "utility.hpp"
#include <array>
#include <cstdlib>

struct Vector
{
    int32_t x;
    int32_t y;
};

using PaperDataRow = std::vector<bool>;
using PaperData = std::vector<PaperDataRow>;

enum class FoldDirection
{
    Verical,
    Horizontal
};

struct FoldInstruction
{
    FoldDirection dirtection;
    int32_t position;
};

struct Paper
{
    int32_t width;
    int32_t height;
    PaperData data;
    std::vector<FoldInstruction> fold_instructions;
};


Paper parse_input(const std::vector<std::string>& values)
{
    if (values.empty())
    {
        throw aoc::puzzle_exception("Input is empty.");
    }

    Paper paper{};

    std::vector<Vector> dots;

    auto it = values.begin();
    while (it != values.end() && !it->empty())
    {
        const auto& row = *it;
        const auto pos = row.find_first_of(',');
        const auto x = std::atoi(row.substr(0, pos).c_str());
        const auto y = std::atoi(row.substr(pos + 1).c_str());
        dots.push_back(Vector{ x, y });

        if (x + 1 > paper.width)
        {
            paper.width = x + 1;
        }
        if (y + 1 > paper.height)
        {
            paper.height = y + 1;
        }

        it++;
    }

    paper.data.resize(paper.height);
    for (auto& row : paper.data)
    {
        row.resize(paper.width);
    }

    for (const auto dot : dots)
    {
        paper.data[dot.y][dot.x] = true;
    }

    ++it;

    while (it != values.end() && !it->empty())
    {
        auto row = *it;
        auto pos = row.find_first_of('=');
        const auto fold_position = std::atoi(row.substr(pos + 1).c_str());
        row = row.substr(0, pos);
        pos = row.find_last_of(' ');
        const auto fold_direction = row.substr(pos + 1) == "x" ? FoldDirection::Verical : FoldDirection::Horizontal;
        paper.fold_instructions.push_back(FoldInstruction{ fold_direction, fold_position });
        it++;
    }

    return paper;
}

void fold_paper_horizonal(Paper& paper, const int32_t position)
{
    auto cut = PaperData(paper.data.begin() + position + 1, paper.data.end());
    paper.data.resize(position);
    paper.height = paper.data.size();

    if (cut.size() > paper.data.size())
    {
        throw aoc::puzzle_exception("Cut is larger than remaining paper.");
    }

    for (size_t y = 0; y < cut.size(); y++)
    {
        size_t paper_y = paper.data.size() - 1 - y;

        for (size_t x = 0; x < paper.width; x++)
        {
            if (cut[y][x])
            {
                paper.data[paper_y][x] = true;
            }
        }
    }

}

void fold_paper_vertical(Paper& paper, const int32_t position)
{
    PaperData cut;
    cut.reserve(paper.height);

    for (auto& row : paper.data)
    {
        cut.push_back(PaperDataRow{ row.begin() + position + 1, row.end() });
        row.resize(position);
    }
    paper.width = position;

    for (size_t y = 0; y < cut.size(); y++)
    {
        for (size_t x = 0; x < cut[y].size(); x++)
        {
            size_t paper_x = paper.data[y].size() - 1 - x;
            if (cut[y][x])
            {
                paper.data[y][paper_x] = true;
            }
        }
    }
}

void fold_paper(Paper& paper, const FoldInstruction& fold_instruction)
{
    switch (fold_instruction.dirtection)
    {
    case FoldDirection::Horizontal: fold_paper_horizonal(paper, fold_instruction.position); break;
    case FoldDirection::Verical: fold_paper_vertical(paper, fold_instruction.position); break;
    }
}

size_t count_dots(const Paper& papper)
{
    size_t count = 0;
    for (const auto& row : papper.data)
    {
        for (const auto& dot : row)
        {
            if (dot)
            {
                ++count;
            }
        }
    }
    return count;
}

size_t solve_part_1(const std::vector<std::string>& values)
{
    auto paper = parse_input(values);
    fold_paper(paper, paper.fold_instructions[0]);
    return count_dots(paper);
}

void print_map(const Paper& papper)
{
    for (const auto& row : papper.data)
    {
        for (const auto& dot : row)
        {
            std::cout << (dot ? '#' : '.');
        }
        std::cout << "\n";
    }
}

size_t solve_part_2(const std::vector<std::string>& values)
{
    auto paper = parse_input(values);
    for (const auto& fold_instruction : paper.fold_instructions)
    {
        fold_paper(paper, fold_instruction);
    }
    print_map(paper);
    return 0;
}

int main() {
    aoc::solve_puzzles(
        aoc::read_input<13, std::string>(),
        solve_part_1,
        solve_part_2
    );
}