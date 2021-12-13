#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <functional>

std::pair<char, size_t> extract_to_fold(const std::string &line)
{
    std::stringstream ss(line);
    std::string line_part;
    getline(ss, line_part, '=');
    char coord_1_name = line_part.back();
    getline(ss, line_part);
    return {coord_1_name, atoi(line_part.c_str())};
}

std::pair<std::set<std::pair<size_t, size_t>>, std::vector<std::pair<char, size_t>>> read_dots()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::set<std::pair<size_t, size_t>> dots;
    std::string tmp;
    while (getline(input, tmp))
    {
        if (!tmp.size())
        {
            break;
        }
        std::stringstream ss(tmp);
        std::string n;
        getline(ss, n, ',');
        size_t x = atoi(n.c_str());
        getline(ss, n);
        size_t y = atoi(n.c_str());
        dots.emplace(x, y);
    }

    std::vector<std::pair<char, size_t>> f;
    while (getline(input, tmp))
    {
        auto f1 = extract_to_fold(tmp);
        f.push_back(f1);
    }
    input.close();

    return {dots, f};
}

void fold_y(std::set<std::pair<size_t, size_t>> &dots, const size_t coord)
{
    std::set<std::pair<size_t, size_t>> new_dots;
    for (const auto &dot : dots)
    {
        size_t y = dot.second > coord ? dot.second - 2 * (dot.second - coord) : dot.second;
        new_dots.insert({dot.first, y});
    }
    dots = new_dots;
}

void fold_x(std::set<std::pair<size_t, size_t>> &dots, const size_t coord)
{
    std::set<std::pair<size_t, size_t>> new_dots;
    for (const auto &dot : dots)
    {
        size_t x = dot.first > coord ? dot.first - 2 * (dot.first - coord) : dot.first;
        new_dots.insert({x, dot.second});
    }
    dots = new_dots;
}

void fold(std::set<std::pair<size_t, size_t>> &dots, const std::pair<char, size_t> &f)
{
    switch (f.first)
    {
    case 'x':
        fold_x(dots, f.second);
        break;
    case 'y':
        fold_y(dots, f.second);
        break;
    default:
        std::cout << "ERROR\n";
        exit(1);
        break;
    }
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto [dots, f] = read_dots();
    fold(dots, f.front());

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << dots.size() << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

void print(const std::set<std::pair<size_t, size_t>> &dots)
{
    for (size_t j = 0; j <= (--dots.end())->second; j++)
    {
        for (size_t i = 0; i <= (--dots.end())->first; i++)
        {
            if (dots.find({i, j}) != dots.end())
            {
                std::cout << '#';
            }
            else
            {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
}

void part_2()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto [dots, f] = read_dots();

    for (const auto &c : f)
    {
        fold(dots, c);
    }
    std::cout << "Part 2: " << '\n';
    print(dots);

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}