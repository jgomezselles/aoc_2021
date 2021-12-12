#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <cctype>

using connections = std::map<std::string, std::set<std::string>>;
const std::string start{"start"}, end{"end"};

connections read_connections()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string tmp;
    connections c;
    while (getline(input, tmp))
    {
        std::stringstream ss(tmp);
        std::string f, s;
        getline(ss, f, '-');
        getline(ss, s);
        c[f].insert(s);
        c[s].insert(f);
    }
    input.close();
    return c;
}

void print_path(const std::vector<std::string> &p)
{
    for (const auto &s : p)
    {
        std::cout << s << ",";
    }
    std::cout << '\n';
}

bool already_passed(const std::vector<std::string> &past, const std::string &to_check)
{
    for (const auto &cave : past)
    {
        if (to_check == cave)
        {
            return true;
        }
    }
    return false;
}

size_t find_paths(const connections &c, std::vector<std::string> past, const std::string &from)
{
    if (from == end)
    {
        return 1;
    }

    if (std::islower(from.front()) && already_passed(past, from))
    {
        return 0;
    }

    past.push_back(from);
    size_t poss{0};
    const auto &neighs = c.at(from);
    for (const auto &to : neighs)
    {
        poss += find_paths(c, past, to);
    }

    return poss;
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    connections c = read_connections();
    size_t n_paths = find_paths(c, {}, start);

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << n_paths << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

size_t find_paths_2(const connections &c, std::vector<std::string> past, const std::string &from, bool has_revisited)
{
    if (from == end)
    {
        return 1;
    }

    if (from == start && past.size())
    {
        return 0;
    }

    if (std::islower(from.front()))
    {
        if (has_revisited)
        {
            if (already_passed(past, from))
            {
                return 0;
            }
        }
        else
        {
            has_revisited = already_passed(past, from);
        }
    }

    past.push_back(from);
    size_t poss{0};
    const auto &neighs = c.at(from);
    for (const auto &to : neighs)
    {
        poss += find_paths_2(c, past, to, has_revisited);
    }

    return poss;
}

void part_2()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    const connections c = read_connections();
    size_t n_paths = find_paths_2(c, {}, start, false);

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 2: " << n_paths << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}