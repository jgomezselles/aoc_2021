#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <optional>

std::optional<uint32_t> sum_next_three(std::vector<uint32_t>::iterator it, std::vector<uint32_t>::iterator end)
{
    uint32_t sum = *it;

    if(++it != end)
    {
        sum+=*it;
    }
    else
    {
        return std::nullopt;
    }

    if(++it != end)
    {
        sum+=*it;
    }
    else
    {
        return std::nullopt;
    }

    return sum;
}

int main()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return {};
    }

    std::vector<uint32_t> numbers;
    std::string line;
    while (std::getline(input, line))
    {
        numbers.push_back(atoi(line.c_str()));
    }

    input.close();

    uint32_t increased{0}, last_sum{std::numeric_limits<uint32_t>::max()};
    for(auto it = numbers.begin(); it != numbers.end(); ++it)
    {
        const auto new_sum = sum_next_three(it, numbers.end());
        if(!new_sum)
        {
            break;
        }

        if(*new_sum > last_sum)
        {
            ++increased;
        }
        last_sum = *new_sum;
    }

    std::cout << "Got " << increased << " increased triplets." << std::endl;

    return 0;
}