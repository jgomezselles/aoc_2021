#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <map>
#include <vector>
#include <algorithm>

size_t count_1_4_7_8s()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    size_t count{0};
    std::string line;
    while (getline(input, line))
    {
        std::stringstream ss(line);
        getline(ss, line, '|');
        getline(ss, line);
        std::stringstream ss2(line);
        while (getline(ss2, line, ' '))
        {
            if (line.size() == 2 || line.size() == 3 || line.size() == 4 || line.size() == 7)
            {
                ++count;
            }
        }
    }
    input.close();
    return count;
}

void part_1()
{
    const auto start = std::chrono::high_resolution_clock::now();
    size_t unique = count_1_4_7_8s();

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 1. Unique signals: " << unique << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

// https://github.com/tminor/aoc-2021-clj/blob/main/src/submarine/day8.clj
std::map<size_t, size_t> hash_map_encoding{
    {467889, 0},
    {89, 1},
    {47788, 2},
    {77889, 3},
    {6789, 4},
    {67789, 5},
    {467789, 6},
    {889, 7},
    {4677889, 8},
    {677889, 9}};

size_t solve_part_2()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }

    std::string line;
    size_t result{0};
    while (getline(input, line))
    {
        std::stringstream ss(line);
        getline(ss, line, '|');
        std::map<char, size_t> freqs{{'a', 0}, {'b', 0}, {'c', 0}, {'d', 0}, {'e', 0}, {'f', 0}, {'g', 0}};
        std::stringstream ss2(line);
        while (getline(ss2, line, ' '))
        {
            for (const auto &c : line)
            {
                ++freqs.at(c);
            }
        }

        getline(ss, line);
        std::stringstream ss3(line);
        std::string panel;
        getline(ss3, line, ' ');
        while (getline(ss3, line, ' '))
        {
            std::string decoded;
            for (const auto &c : line)
            {
                decoded.append(std::to_string(freqs.at(c)));
            }
            std::sort(decoded.begin(), decoded.end());
            panel.append(std::to_string(hash_map_encoding.at(std::stoi(decoded))));
        }
        result += std::stoi(panel);
    }
    input.close();
    return result;
}

void part_2()
{
    const auto start = std::chrono::high_resolution_clock::now();
    size_t unique = solve_part_2();

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 2: " << unique << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}