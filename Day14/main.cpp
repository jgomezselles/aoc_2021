#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <sstream>
#include <unordered_map>

std::pair<std::string, std::map<std::pair<char, char>, char>> read_input()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string polymer, tmp;
    getline(input, polymer);
    std::string line;
    getline(input, line);
    std::map<std::pair<char, char>, char> rules;
    while (getline(input, line))
    {
        std::stringstream ss(line);
        std::string first, second;
        getline(ss, first, ' ');
        getline(ss, second, ' ');
        getline(ss, second);
        rules.insert({{*first.begin(), *(first.begin() + 1)}, *second.begin()});
    }
    input.close();

    return {polymer, rules};
}

std::string process_step(const std::string &polymer, const std::map<std::pair<char, char>, char> &rules)
{
    std::string new_polymer;
    auto it = polymer.begin();
    for (it; it != polymer.end() - 1; ++it)
    {
        new_polymer += *it;
        const std::pair<char, char> pair{*it, *(it + 1)};
        if (auto rit = rules.find(pair); rit != rules.end())
        {
            new_polymer += rit->second;
        }
    }
    new_polymer += *it;
    return new_polymer;
}

size_t res_1(const std::string &polymer)
{
    std::unordered_map<char, size_t> counts;
    for (const auto &c : polymer)
    {
        counts[c] += 1;
    }

    std::size_t max{0}, min{std::numeric_limits<size_t>::max()};
    for (const auto &[c, n] : counts)
    {
        max = n > max ? n : max;
        min = n < min ? n : min;
    }
    return max - min;
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto [polymer, rules] = read_input();

    for (size_t i = 1; i <= 10; i++)
    {
        polymer = process_step(polymer, rules);
    }

    const auto res = res_1(polymer);
    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

std::map<std::pair<char, char>, size_t> process_step_2(const std::string &polymer, const std::map<std::pair<char, char>, char> &rules, const size_t steps)
{
    std::map<std::pair<char, char>, size_t> counters;
    for (auto it = polymer.begin(); it != polymer.end() - 1; ++it)
    {
        counters[{*it, *(it + 1)}] += 1;
    }

    for (int i = 0; i < steps; i++)
    {
        std::map<std::pair<char, char>, size_t> new_counters;
        for (auto &[pair, n] : counters)
        {
            new_counters[{pair.first, rules.at(pair)}] += n;
            new_counters[{rules.at(pair), pair.second}] += n;
            n = 0;
        }

        for (const auto &[k,v] : new_counters)
        {
            counters[k] += v;
        }
    }
    return counters;
}

size_t res_2(const std::map<std::pair<char, char>, size_t> &counters, const std::string &polymer)
{
    std::unordered_map<char, size_t> counts{{polymer.back(), 1}};
    for (const auto &c : counters)
    {
        counts[c.first.first] += c.second;
    }

    std::size_t max{0}, min{std::numeric_limits<size_t>::max()};
    for (const auto &p : counts)
    {
        min = std::min(min, p.second);
        max = std::max(max, p.second);
    }
    return max - min;
}

void part_2()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto [polymer, rules] = read_input();

    const auto counters = process_step_2(polymer, rules, 40);

    const auto res = res_2(counters, polymer);
    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 2: " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}