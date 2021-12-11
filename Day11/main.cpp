#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <algorithm>

std::vector<std::vector<size_t>> read_energies()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::vector<std::vector<size_t>> energies;
    std::string line;
    while (getline(input, line))
    {
        std::vector<size_t> l;
        for (const auto &c : line)
        {
            l.push_back(c - '0');
        }
        energies.push_back(l);
    }
    input.close();
    return energies;
}

void all_plus_one(std::vector<std::vector<size_t>> &energies)
{
    for (auto &l : energies)
    {
        for (auto &e : l)
        {
            e += 1;
        }
    }
}

size_t get_min(const size_t i)
{
    return i == 0 ? 0 : i - 1;
}

size_t get_max(const size_t i)
{
    return i == 9 ? 9 : i + 1;
}

void flash(std::vector<std::vector<size_t>> &energies, const size_t i, const size_t j, size_t &flashes);

void process_neighbors(std::vector<std::vector<size_t>> &energies, const size_t i, const size_t j, size_t &flashes)
{
    std::vector<std::pair<size_t, size_t>> to_flash;
    for (size_t ni = get_min(i); ni <= get_max(i); ni++)
    {
        for (size_t nj = get_min(j); nj <= get_max(j); nj++)
        {
            if (size_t &e = energies[ni][nj]; e != 0 && e < 9)
            {
                ++e;
            }
            else if (e == 9)
            {
                flash(energies, ni, nj, flashes);
            }
        }
    }
}

void flash(std::vector<std::vector<size_t>> &energies, const size_t i, const size_t j, size_t &flashes)
{
    if (auto &e = energies[i][j]; e >= 9)
    {
        e = 0;
        ++flashes;
        process_neighbors(energies, i, j, flashes);
    }
}

std::vector<std::pair<size_t, size_t>> get_to_flash(const std::vector<std::vector<size_t>> &energies)
{
    std::vector<std::pair<size_t, size_t>> to_flash;
    for (size_t i = 0; i < energies.size(); i++)
    {
        for (size_t j = 0; j < energies[i].size(); j++)
        {
            if (const auto &e = energies[i][j]; e > 9)
            {
                to_flash.push_back({i, j});
            }
        }
    }
    return to_flash;
}

void print_energies(const std::vector<std::vector<size_t>> &energies)
{
    for (const auto &v : energies)
    {
        for (const auto &e : v)
        {
            std::cout << e;
        }
        std::cout << '\n';
    }
}

size_t get_flashes(const size_t n_steps)
{
    size_t flashes{0};
    auto energies = read_energies();
    for (size_t step = 1; step <= n_steps; ++step)
    {
        size_t old_flashes = flashes;
        all_plus_one(energies);

        for (const auto [i, j] : get_to_flash(energies))
        {
            flash(energies, i, j, flashes);
        }

        if (flashes - old_flashes == 100)
        {
            std::cout << "Hey!" << step << '\n';
        }
    }
    return flashes;
}

void part_1()
{
    const auto start = std::chrono::high_resolution_clock::now();
    const auto flashes = get_flashes(100);

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Flashes after 100 " << flashes << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

size_t get_super_flash()
{
    size_t flashes{0}, step{0};
    auto energies = read_energies();

    while (true)
    {
        ++step;

        size_t old_flashes = flashes;
        all_plus_one(energies);

        for (const auto [i, j] : get_to_flash(energies))
        {
            flash(energies, i, j, flashes);
        }

        if (flashes - old_flashes == 100)
        {
            break;
        }
    }

    return step;
}

void part_2()
{
    const auto start = std::chrono::high_resolution_clock::now();
    const auto flashes = get_super_flash();

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "All flashed after n_steps: " << flashes << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}