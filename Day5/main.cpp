#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <map>
#include <tuple>

using coord = std::pair<size_t, size_t>;
using segment = std::pair<coord, coord>;
using map = std::map<coord, size_t>;


std::tuple<size_t, size_t, size_t, size_t> extract_segment(const std::string &line)
{
    std::stringstream ss(line);
    std::string substr;
    std::getline(ss, substr, ',');
    size_t x1 = atoi(substr.c_str());
    std::getline(ss, substr, ' ');
    size_t y1 = atoi(substr.c_str());
    std::getline(ss, substr, ' ');
    std::getline(ss, substr, ',');
    size_t x2 = atoi(substr.c_str());
    std::getline(ss, substr);
    size_t y2 = atoi(substr.c_str());

    return {x1, y1, x2, y2};
}

void add_coord(map &vent_map, const coord &c)
{
    auto it = vent_map.find(c);
    if (it != vent_map.end())
    {
        it->second++;
    }
    else
    {
        vent_map.insert({std::move(c), 1});
    }
}

map read_input_1()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }

    map vent_map;
    std::string line;
    while (std::getline(input, line))
    {
        auto [x1, y1, x2, y2] = extract_segment(line);

        if (x1 == x2)
        {
            for (size_t j = std::min(y1, y2); j <= std::max(y1, y2); ++j)
            {
                add_coord(vent_map, {x1, j});
            }
        }
        else if (y1 == y2)
        {
            for (size_t i = std::min(x1, x2); i <= std::max(x1, x2); ++i)
            {
                add_coord(vent_map, {i, y1});
            }
        }
    }
    input.close();
    return std::move(vent_map);
}

size_t two_or_larger(const map &vent_map)
{
    size_t count{0};
    for (auto &[_, n] : vent_map)
    {
        if (n > 1)
        {
            ++count;
        }
    }
    return count;
}

void part_1()
{
    std::cout << "==========>PART 1<===========" << std::endl;
    const auto start = std::chrono::high_resolution_clock::now();

    map vent_map = read_input_1();
    size_t res = two_or_larger(vent_map);

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 1. Score is: " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

bool get_next_component(size_t &current, const size_t &init, const size_t &end)
{
    if (current == end)
    {
        return false;
    }

    current < end ? current += 1 : current -= 1;
    return true;
}

bool get_next_coord(coord &current, const coord &init, const coord &end)
{
    bool b1 = get_next_component(current.first, init.first, end.first);
    bool b2 = get_next_component(current.second, init.second, end.second);
    return b1 || b2;
}

map read_input_2()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }

    map vent_map;
    std::string line;
    while (std::getline(input, line))
    {
        auto [x1, y1, x2, y2] = extract_segment(line);
        coord init{x1, y1}, end{x2, y2}, current(init);
        add_coord(vent_map, current);
        while (get_next_coord(current, init, end))
        {
            add_coord(vent_map, current);
        }
    }
    input.close();
    return std::move(vent_map);
}

void part_2()
{
    std::cout << "==========>PART 2<===========" << std::endl;
    const auto start = std::chrono::high_resolution_clock::now();

    map vent_map = read_input_2();
    size_t res = two_or_larger(vent_map);

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 2. Score is: " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}