#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <sstream>
#include <bitset>

std::string read_input()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string line, input_data;
    getline(input, line);

    for (const auto &c : line)
    {
        size_t n;
        std::istringstream({c}) >> std::hex >> n;
        input_data.append(std::bitset<4>(n).to_string());
    }

    input.close();
    return input_data;
}

size_t extract_n(std::string &str, const size_t n)
{
    std::string v = str.substr(0, n);
    str.erase(0, n);
    return std::bitset<64>(v).to_ulong();
}

size_t extract_one(std::string &str)
{
    size_t v = str.front() - '0';
    str.erase(0, 1);
    return v;
}

std::size_t extract_lit_dec(std::string &str)
{
    bool b_cont = true;
    std::string lit_str;
    while (b_cont)
    {
        std::string processing = str.substr(0, 5);
        str.erase(0, 5);
        if (processing.front() - '0' == 0)
        {
            b_cont = false;
        }
        lit_str.append(processing.substr(1));
    }
    return std::bitset<64>(lit_str).to_ulong();
}

size_t extract_operator(std::string &str);

size_t extract_subpacket(std::string &str)
{
    size_t version = extract_n(str, 3);
    if (extract_n(str, 3) == 4)
    {
        extract_lit_dec(str);
    }
    else
    {
        version += extract_operator(str);
    }
    return version;
}

size_t extract_n_subpackets(std::string &str, const size_t n)
{
    size_t versions{0};
    for (size_t i = 0; i < n; i++)
    {
        versions += extract_subpacket(str);
    }
    return versions;
}

size_t extract_l_subpackets(std::string &str, const size_t l)
{
    size_t versions{0};
    std::string subpackets = str.substr(0, l);
    str.erase(0, l);
    while (subpackets.size())
    {
        versions += extract_subpacket(subpackets);
    }
    return versions;
}

size_t extract_operator(std::string &str)
{
    std::size_t versions{0};
    switch (extract_one(str))
    {
    case 0:
        versions += extract_l_subpackets(str, std::bitset<15>(extract_n(str, 15)).to_ulong());
        break;
    case 1:
        versions += extract_n_subpackets(str, std::bitset<11>(extract_n(str, 11)).to_ulong());
        break;
    default:
        std::cout << "Error! " << std::endl;
        exit(1);
        break;
    }
    return versions;
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto input = read_input();
    size_t versions{0};
    while (input.size())
    {
        versions += extract_subpacket(input);
        if (size_t one_pos = input.find_first_of('1'); one_pos == input.npos)
        {
            break;
        }
    }

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << versions << '\n';
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