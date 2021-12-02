#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

const std::string fwd = "forward";
const std::string down = "down";
const std::string up = "up";

void process_line(const std::string &line, std::tuple<int, int, int> &result)
{
    // std::cout << "processing line " << line << std::endl;
    auto &[hori, depth, aim] = result;
    std::stringstream ss(line);

    std::string instruction, amount;
    std::getline(ss, instruction, ' ');
    std::getline(ss, amount);

    int int_am = atoi(amount.c_str());

    if (instruction == down)
    {
        aim += int_am;
    }
    else if (instruction == up)
    {
        aim -= int_am;
    }
    else if (instruction == fwd)
    {
        hori += int_am;
        depth += aim*int_am;
    }

    if (depth < 0)
    {
        std::cout << "Out of the water!!!" << std::endl;
        depth = 0;
    }
}

std::tuple<int, int, int> process_instructions()
{
    std::ifstream input("input_test.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return {};
    }

    std::tuple<int, int, int> res{0,0,0};
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
    auto [horizontal, depth, aim] = process_instructions();
    std::cout << "H: " << horizontal << ", "
              << "Depth: " << depth << std::endl;
    std::cout << "Result is " << horizontal * depth << std::endl;
    return 0;
}