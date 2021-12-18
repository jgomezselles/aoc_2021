#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <chrono>
#include <map>
#include <sstream>
#include <optional>
#include <algorithm>

using s_number = std::deque<std::string>;

void print(const s_number &n)
{
    for (const auto &c : n)
    {
        std::cout << c;
    }
    std::cout << '\n';
};

s_number line_to_s_number(const std::string &line)
{
    s_number n;
    for (auto c : line)
    {
        n.push_back(std::string(1, c));
    }
    return n;
}

s_number::iterator to_explode(s_number &s)
{
    size_t open{0};
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        if (*it == "[")
        {
            open += 1;
            if (open == 5)
            {
                return ++it;
            }
        }
        else if (*it == "]")
        {
            open -= 1;
        }
    }
    return s.end();
}

std::optional<s_number::iterator> prev_number(s_number &s, const s_number::iterator &it)
{
    std::optional<s_number::iterator> res;
    for (auto pit = s.begin(); pit != it; ++pit)
    {
        if (std::isdigit(pit->front()))
        {
            res = pit;
        }
    }
    return res;
}

std::optional<s_number::iterator> next_number(s_number &s, const s_number::iterator &it)
{
    for (auto pit = it + 1; pit != s.end(); ++pit)
    {
        if (std::isdigit(pit->front()))
        {
            return pit;
        }
    }
    return std::nullopt;
}

bool is_pair(const s_number &s, const s_number::const_iterator it)
{
    return s.size() > it - s.begin() + 3 && *(it + 1) == "," && std::isdigit((it + 2)->front());
}

void reduce(s_number &s);

void explode(s_number &s, const s_number::iterator it)
{
    auto prev_it = prev_number(s, it);
    if (prev_it.has_value())
    {
        **prev_it = std::to_string(std::stoul(*prev_it.value()) + std::stoul(*it));
    }

    auto next_it = next_number(s, it + 2);
    if (next_it.has_value())
    {
        **next_it = std::to_string(atoi(next_it.value()->c_str()) + atoi((it + 2)->c_str()));
    }

    size_t zero_pos = it - 1 - s.begin();
    s.erase(it - 1, it + 4);
    s.insert(s.begin() + zero_pos, "0");
}

void split(s_number &s, const s_number::iterator it)
{
    size_t min{std::stoul(*it) / 2}, max = std::stoul(*it) % 2 ? min + 1 : min;
    std::vector<std::string> v = {"[", std::to_string(min), ",", std::to_string(max), "]"};
    size_t insert_pos = it - s.begin();
    s.erase(it);
    s.insert(s.begin() + insert_pos, v.begin(), v.end());
}

auto to_split = [](const std::string &str)
{ return std::isdigit(str.front()) && std::stoul(str) > 9; };

void reduce(s_number &s)
{
    if (auto it = to_explode(s); it != s.end() && is_pair(s, it))
    {
        explode(s, it);
        reduce(s);
    }

    if (auto it = std::ranges::find_if(s, to_split); it != s.end())
    {
        split(s, it);
        reduce(s);
    }
}

void add(s_number &n, const s_number &s)
{
    n.push_front("[");
    n.push_back(",");
    n.insert(n.end(), s.begin(), s.end());
    n.push_back("]");
    reduce(n);
}

s_number read_input()
{
    std::ifstream input("input_test.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string next_line;
    getline(input, next_line);
    s_number final_number = line_to_s_number(next_line);
    while (getline(input, next_line))
    {
        add(final_number, line_to_s_number(next_line));
    }
    input.close();

    return final_number;
}

s_number::iterator inner_pair_comma(s_number &s)
{
    std::string prev, next;
    for (auto it = s.begin() + 1; it != s.end() - 1; ++it)
    {
        if (*it == "," && isdigit((it + 1)->front()) && isdigit((it - 1)->front()))
        {
            return it;
        }
    }
    std::cout << "ERROR\n";
    exit(1);
    return s.end();
}

void explode_for_magnitude(s_number &s, const s_number::iterator it)
{
    size_t left = std::stoul(*(it - 1)), right = std::stoul(*(it + 1));

    size_t zero_pos = it - 2 - s.begin();
    s.erase(it - 2, it + 3);
    s.insert(s.begin() + zero_pos, std::to_string(3 * left + 2 * right));
}

size_t magnitude(s_number s)
{
    while (s.size() > 1)
    {
        explode_for_magnitude(s, inner_pair_comma(s));
    }

    return std::stoul(s.front());
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto final_line = read_input();
    auto mg = magnitude(final_line);

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << mg << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

std::vector<s_number> read_input_2()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string next_line;
    std::vector<s_number> s_numbers;
    while (getline(input, next_line))
    {
        s_numbers.push_back(line_to_s_number(next_line));
    }
    input.close();

    return s_numbers;
}

std::map<std::pair<size_t, size_t>, size_t>::iterator find_max_magnitude(const std::vector<s_number>& numbers)
{
    std::map<std::pair<size_t, size_t>, size_t> magnitudes;

    for (auto it = numbers.cbegin(); it != numbers.cend(); ++it)
    {
        for (auto jt = numbers.cbegin(); jt != numbers.cend(); ++jt)
        {
            const size_t i{size_t(it - numbers.cbegin())}, j{size_t(jt - numbers.begin())};
            if (magnitudes.find({i, j}) == magnitudes.end())
            {
                auto a = *it;
                add(a, *jt);
                auto tmpmg = magnitude(a);
                magnitudes[{i, j}] = tmpmg;
            }

            if (magnitudes.find({j, i}) == magnitudes.end())
            {
                auto a = *jt;
                add(a, *it);
                auto tmpmg = magnitude(a);
                magnitudes[{j, i}] = tmpmg;
            }
        }
    }

    auto mg = std::max_element(magnitudes.begin(), magnitudes.end(),
                               [](const std::pair<std::pair<size_t, size_t>, size_t> &p1,
                                  const std::pair<std::pair<size_t, size_t>, size_t> &p2)
                               { return p1.second < p2.second; });

    return mg;
}

void part_2()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    const auto numbers = read_input_2();

    const auto mg = find_max_magnitude(numbers);

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 2: "
              << "Magnitude [" << mg->first.first << ',' << mg->first.second << "]: " << mg->second << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}