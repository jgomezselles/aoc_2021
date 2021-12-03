#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <algorithm>

void remove_with_ones(std::vector<std::bitset<12>> &res, const size_t i)
{
    res.erase(std::remove_if(res.begin(), res.end(),
                             [i](std::bitset<12> bs)
                             { return bs[i]; }),
              res.end());
}

void remove_with_zeros(std::vector<std::bitset<12>> &res, const size_t i)
{
    res.erase(std::remove_if(res.begin(), res.end(),
                             [i](std::bitset<12> bs)
                             { return !bs[i]; }),
              res.end());
}

std::pair<size_t, size_t> process_instructions()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return {};
    }

    std::vector<std::bitset<12>> saved;
    std::string line;
    while (std::getline(input, line))
    {
        saved.push_back(std::bitset<12>(line));
    }
    input.close();

    auto o2_copy = saved;
    for (size_t i = 12-1; i >= 0; --i)
    {
        size_t ones{0};
        for (const auto &s : o2_copy)
        {
            ones += s[i];
        }

        if (2 * ones >= o2_copy.size())
        {
            remove_with_zeros(o2_copy, i);
        }
        else
        {
            remove_with_ones(o2_copy, i);
        }

        if (o2_copy.size() == 1)
        {
            break;
        }
    }

    auto co2_copy = saved;
    for (size_t i = 12-1; i >= 0; --i)
    {
        size_t ones{0};
        for (const auto &s : co2_copy)
        {

            ones += s[i];
        }

        if (2 * ones < co2_copy.size())
        {
            remove_with_zeros(co2_copy, i);
        }
        else
        {
            remove_with_ones(co2_copy, i);
        }

        if (co2_copy.size() == 1)
        {
            break;
        }
    }

    return {o2_copy.front().to_ulong(), co2_copy.front().to_ulong()};
}

int main()
{
    auto [o2, co2] = process_instructions();
    std::cout << "o2: " << o2 << ", "
              << "co2: " << co2 << std::endl;
    std::cout << "Result is " << o2 * co2 << std::endl;
    return 0;
}