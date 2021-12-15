#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <sstream>
#include <set>
#include <queue>
#include <cmath>

using point = std::pair<size_t, size_t>;
std::map<point, size_t> read_input()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }

    std::string line;
    std::map<point, size_t> risks;
    size_t i{0};
    while (getline(input, line))
    {
        size_t j{0};
        for (const auto c : line)
        {
            risks.insert({{i, j}, c - '0'});
            ++j;
        }
        ++i;
    }
    input.close();

    return risks;
}

size_t get_min(const size_t i, const size_t min)
{
    return i == min ? min : i - 1;
}

size_t get_max(const size_t i, const size_t max)
{
    return i == max ? max : i + 1;
}

std::vector<point> get_neighbors(const point &p, const size_t min, const size_t max)
{
    std::vector<point> neighs;
    for (size_t ni = get_min(p.first, min); ni <= get_max(p.first, max); ni++)
    {
        neighs.push_back({ni, p.second});
    }

    for (size_t nj = get_min(p.second, min); nj <= get_max(p.second, max); nj++)
    {
        neighs.push_back({p.first, nj});
    }
    return neighs;
}

size_t dijkstra(const std::map<point, size_t> &risks, const point &source, const point &end)
{
    std::priority_queue<std::pair<size_t, point>, std::vector<std::pair<size_t, point>>, std::greater<std::pair<size_t, point>>> Q;
    Q.emplace(0, source);

    std::map<point, size_t> total_risk;
    total_risk[source] = 0;

    std::map<point, point> prev;
    prev[source] = source;

    while (!Q.empty())
    {
        auto u = Q.top().second;
        Q.pop();

        for (const auto &v : get_neighbors(u, source.first, end.first))
        {
            if (size_t alt = total_risk[u] + risks.at(v); total_risk.find(v) == total_risk.end() || alt < total_risk.at(v))
            {
                total_risk[v] = alt;
                size_t prio = alt + abs(u.first - v.first) + abs(u.second - v.second);
                Q.emplace(prio, v);
                prev[v] = u;
            }
        }
    }

    return total_risk[end];
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto risks = read_input();
    auto res = dijkstra(risks, risks.begin()->first, risks.rbegin()->first);

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

void part_2()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto risks = read_input();
    auto high_risks = risks;
    size_t max = risks.rbegin()->first.first + 1;

    for (size_t i = 0; i < 5 * max; i++)
    {
        for (size_t j = 0; j < 5 * max; j++)
        {
            size_t next = risks.at({j % max, i % max}) + i / max + j / max;
            high_risks[{j, i}] = next <= 9 ? next : next - 9;
        }
    }

    auto res = dijkstra(high_risks, high_risks.begin()->first, high_risks.rbegin()->first);

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