#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <sstream>

std::vector<std::pair<std::string, std::string>> read_input()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string line;
    std::vector<std::pair<std::string, std::string>> input_data;
    while (getline(input, line))
    {
        std::stringstream ss(line);
        std::string first, second;
        getline(ss, first, ' ');
        input_data.push_back({first, second});
    }
    input.close();

    return input_data;
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto lines = read_input();

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << 666 << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

void part_2()
{
    return;
}

int main()
{
    part_1();
    part_2();
    return 0;
}