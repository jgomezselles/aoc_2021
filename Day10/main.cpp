#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <algorithm>
#include <optional>

const std::map<char, size_t> chunk_relations{
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'}};

const std::map<char, size_t> rev_chunk_relations{
    {')', '('},
    {']', '['},
    {'}', '{'},
    {'>', '<'}};

const std::map<char, size_t> illegal_scores{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}};

const std::map<char, size_t> completion_scores{
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4}};

std::pair<size_t, size_t> solve_stuff()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }

    std::string line;
    size_t syntax_checker_score{0};
    std::vector<size_t> autocomplete_scores;
    while (getline(input, line))
    {
        std::string stack;
        std::optional<char> first_illegal;
        for (const auto &c : line)
        {
            if (chunk_relations.find(c) != chunk_relations.end())
            {
                stack += c;
            }
            else if (stack.back() == rev_chunk_relations.at(c))
            {
                stack.pop_back();
            }
            else
            {
                first_illegal.emplace(c);
                break;
            }
        }

        if (first_illegal.has_value())
        {
            syntax_checker_score += illegal_scores.at(*first_illegal);
        }
        else
        {
            size_t partial{0};
            for (auto it = stack.rbegin(); it != stack.rend(); ++it)
            {
                partial *= 5;
                partial += completion_scores.at(chunk_relations.at(*it));
            }
            autocomplete_scores.push_back(partial);
        }
    }
    input.close();

    std::sort(autocomplete_scores.begin(), autocomplete_scores.end());

    return {syntax_checker_score, autocomplete_scores.at(autocomplete_scores.size() / 2)};
}

void solve_and_measure()
{
    const auto start = std::chrono::high_resolution_clock::now();
    const auto [part_1, part_2] = solve_stuff();

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 1. " << part_1 << '\n';
    std::cout << "Part 2. " << part_2 << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    solve_and_measure();
    return 0;
}