#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <functional>

const std::string fwd = "forward";
const std::string down = "down";
const std::string up = "up";

void process_line(const std::string &line, std::pair<int, int>& result)
{
    //std::cout << "processing line " << line << std::endl;
    std::stringstream ss(line);

    std::string instruction, amount;
    std::getline(ss, instruction, ' ');
    std::getline(ss, amount);

    int int_am = instruction == up ? -atoi(amount.c_str()) : atoi(amount.c_str());

    if(instruction == fwd)
    {
        result.first += int_am;
    }
    else
    {
        result.second += int_am;
    }

    if(result.second < 0)
    {
        std::cout << "Out of the water!!!" << std::endl;
        result.second = 0;
    }

}


std::pair<int, int> process_instructions()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return {};
    }

    std::pair<int, int> res{0,0};
    std::string line;
    while (std::getline(input, line))
    {
        process_line(line, res);
    }
    input.close();

    return res;
}

int main()
{
    auto [horizontal, depth] = process_instructions();
    std::cout << "H: " << horizontal << ", " << "Depth: " << depth << std::endl;
    std::cout << "Result is " << horizontal*depth << std::endl;
    return 0;
}