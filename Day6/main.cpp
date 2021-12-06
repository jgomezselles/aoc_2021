#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <map>

void fill_map_with_input(std::map<size_t, size_t> &lfm)
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string line;
    std::getline(input, line);
    input.close();
    std::stringstream ss(line);
    while (getline(ss, line, ','))
    {
        ++lfm.at(atoi(line.c_str()));
    }
}

void process(size_t n_days, std::map<size_t, size_t> &lfm)
{
    for (size_t d = 1; d <= n_days; ++d)
    {
        auto new_lfm = lfm;
        for (auto &[k, v] : lfm)
        {
            if (k == 0)
            {
                new_lfm.at(6) += v;
                new_lfm.at(8) += v;
            }
            else
            {
                new_lfm.at(k-1) += v;
            }

            new_lfm.at(k) -= v;
        }
        lfm = new_lfm;
    }
}

void part_1()
{
    std::map<size_t, size_t> lantern_fish_map;
    for (size_t i = 0; i <= 8; ++i)
    {
        lantern_fish_map.insert({i, 0});
    }

    fill_map_with_input(lantern_fish_map);
    size_t n_days;
    std::cout << "How many days?\n";
    std::cin >> n_days;

    const auto start = std::chrono::high_resolution_clock::now();
    process(n_days, lantern_fish_map);

    size_t n_fish{0};
    std::cout << "Printing status after " << n_days << '\n';
    for(const auto& [k,v] : lantern_fish_map)
    {
        n_fish += v;
        std::cout << k << " -> " << v <<'\n';
    }
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "N fish is: " << n_fish << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    return 0;
}