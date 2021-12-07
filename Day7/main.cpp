#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>
#include <chrono>

void fill_mset_with_input(std::vector<size_t> &crabs)
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
        crabs.push_back(atoi(line.c_str()));
    }
    std::sort(crabs.begin(), crabs.end());
}

// https://stackoverflow.com/questions/1719070/what-is-the-right-approach-when-using-stl-container-for-median-calculation/1719155#1719155
size_t compute_fuel_to_median(std::vector<size_t> &crabs)
{
    size_t n = crabs.size() / 2;
    std::nth_element(crabs.begin(), crabs.begin() + n, crabs.end());
    size_t target = crabs[n];
    size_t fuel = std::accumulate(crabs.begin(), crabs.end(), 0,
                    [target](size_t accumulated, size_t value)
                    {
                        return accumulated + abs(value - target);
                    });
    return fuel;
}

void part_1()
{
    std::vector<size_t> crabs;
    fill_mset_with_input(crabs);

    const auto start = std::chrono::high_resolution_clock::now();
    size_t fuel = compute_fuel_to_median(crabs);

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 1. Fuel spent is: " << fuel << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

size_t calc_fuel2(const std::vector<size_t> &crabs, size_t target)
{
    return std::accumulate(crabs.begin(), crabs.end(), 0,
                           [target](size_t accumulated, size_t value)
                           {
                               return accumulated + abs(value - target) * (abs(value - target) + 1) / 2;
                           });
}

void part_2()
{
    std::vector<size_t> crabs;
    fill_mset_with_input(crabs);

    const auto start = std::chrono::high_resolution_clock::now();

    size_t avg = std::accumulate(crabs.cbegin(), crabs.cend(), 0) / crabs.size();
    long fuel2 = std::min(
        calc_fuel2(crabs, avg + 1),
        calc_fuel2(crabs, avg));

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 2. Fuel spent is: " << fuel2 << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}