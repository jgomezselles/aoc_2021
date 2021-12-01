#include <iostream>
#include <fstream>
#include <string>
#include <limits>

int main()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return {};
    }

    uint32_t increased{0}, last_num{std::numeric_limits<uint32_t>::max()};

    std::string line;
    while (std::getline(input, line))
    {
        const auto new_num = atoi(line.c_str());
        if(new_num > last_num)
        {
            ++increased;
        }
        last_num = new_num;
    }

    input.close();

    std::cout << "Got " << increased << " increased numbers." << std::endl;

    return 0;
}