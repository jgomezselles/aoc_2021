#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>

std::pair<size_t, size_t> process_instructions()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return {};
    }

    std::bitset<12> gamma, eps;
    std::vector<std::bitset<12>> saved;
    std::string line;
    while (std::getline(input, line))
    {
        saved.push_back(std::bitset<12>(line));
    }

    for(size_t i = 0; i < 12; ++i)
    {
        size_t ones{0};
        for(const auto& s : saved)
        {
            ones += s[i];
        }
        std::cout << "Ones: " << ones << std::endl;

        if(2*ones < saved.size())
        {
            eps.set(i);
        }
        else
        {
            gamma.set(i);
        }
    }

    input.close();

    return {gamma.to_ulong(), eps.to_ulong()};
}

int main()
{
    auto [gamma, eps] = process_instructions();
    std::cout << "gamma: " << gamma << ", " << "Epsilon: " << eps << std::endl;
    std::cout << "Result is " << gamma*eps << std::endl;
    return 0;
}