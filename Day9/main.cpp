#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <map>
#include <vector>
#include <algorithm>
#include <set>

std::vector<std::vector<size_t>> read_heatmap()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::vector<std::vector<size_t>> heatmap;
    std::string line;
    while (getline(input, line))
    {
        std::vector<size_t> hml; // heatmapline
        for (const auto &c : line)
        {
            hml.push_back(c - '0');
        }
        heatmap.push_back(hml);
    }
    input.close();

    return heatmap;
}

std::set<std::pair<size_t, size_t>> valid_indexes(const size_t i, const size_t j, const std::vector<std::vector<size_t>> &heatmap)
{
    std::set<std::pair<size_t, size_t>> valid;

    if (j > 0)
    {
        valid.insert({i, j - 1});
    }

    if (j < heatmap[i].size() - 1)
    {
        valid.insert({i, j + 1});
    }

    if (i > 0)
    {
        valid.insert({i - 1, j});
    }

    if (i < heatmap.size() - 1)
    {
        valid.insert({i + 1, j});
    }

    return valid;
}

std::vector<std::pair<size_t, size_t>> get_low_points(const std::vector<std::vector<size_t>> &heatmap)
{
    std::vector<std::pair<size_t, size_t>> low_points;
    for (size_t i = 0; i < heatmap.size(); i++)
    {
        for (size_t j = 0; j < heatmap[i].size(); j++)
        {
            std::set<size_t> adj;
            const auto &n = heatmap[i][j];
            for (const auto &[k, v] : valid_indexes(i, j, heatmap))
            {
                adj.insert(heatmap[k][v]);
            }

            if (n < *adj.begin())
            {
                low_points.push_back({i, j});
            }
        }
    }
    return low_points;
}

size_t solve_part_1()
{
    auto heatmap = read_heatmap();
    size_t total_risk{0};
    for (const auto &[i, j] : get_low_points(heatmap))
    {
        total_risk += 1 + heatmap[i][j];
    }

    return total_risk;
}

void part_1()
{
    const auto start = std::chrono::high_resolution_clock::now();
    size_t res = solve_part_1();

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 1. " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

bool grow_basin(const std::vector<std::vector<size_t>> &heatmap, std::set<std::pair<size_t, size_t>> &basin, const std::set<std::pair<size_t, size_t>> &to_explore)
{
    std::set<std::pair<size_t, size_t>> new_to_explore;
    for (const auto &[i, j] : to_explore)
    {
        for (const auto &valid : valid_indexes(i, j, heatmap))
        {
            if (auto it = basin.find(valid); it == basin.end() && heatmap[valid.first][valid.second] != 9 && heatmap[valid.first][valid.second] > heatmap[i][j])
            {
                new_to_explore.insert(valid);
                basin.insert(valid);
            }
        }
    }
    return new_to_explore.size() ? grow_basin(heatmap, basin, new_to_explore) : false;
}

size_t solve_part_2()
{
    auto heatmap = read_heatmap();
    auto low_points = get_low_points(heatmap);
    std::vector<size_t> basin_sizes;
    for (const auto &[i, j] : low_points)
    {
        std::set<std::pair<size_t, size_t>> basin;
        basin.insert({i, j});
        grow_basin(heatmap, basin, basin);
        basin_sizes.push_back(basin.size());
    }
    std::sort(basin_sizes.begin(), basin_sizes.end());
    auto n_basins = basin_sizes.size();

    return basin_sizes[n_basins - 1] * basin_sizes[n_basins - 2] * basin_sizes[n_basins - 3];
}

void part_2()
{
    const auto start = std::chrono::high_resolution_clock::now();
    size_t res = solve_part_2();

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 2. " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}