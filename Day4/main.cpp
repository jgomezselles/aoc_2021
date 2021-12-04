#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <chrono>

using board = std::vector<std::vector<std::pair<size_t, bool>>>;

void print_board(board b)
{
    std::cout << "==========>Printing board<===========" << std::endl;
    std::stringstream ss;
    for (const auto &r : b)
    {
        for (const auto &n : r)
        {
            ss << n.first << " ";
        }
        ss << '\n';
    }
    std::cout << ss.str();
}

std::vector<board> read_boards(std::ifstream &input)
{
    std::vector<board> boards;
    std::string line;
    board b;
    while (std::getline(input, line))
    {
        if (line.empty())
        {
            if (b.size())
            {
                boards.push_back(b);
                b.clear();
            }
            continue;
        }

        std::vector<std::pair<size_t, bool>> row;
        std::stringstream ss_r(line);
        std::string str_number;
        while (std::getline(ss_r, str_number, ' '))
        {
            if (str_number.length() > 0)
            {
                row.push_back({atoi(str_number.c_str()), false});
            }
        }
        b.push_back(row);
    }

    boards.push_back(b);
    return boards;
}

std::vector<size_t> read_drawn(std::ifstream &input)
{
    std::vector<size_t> drawn;
    std::string line;
    std::getline(input, line);
    std::stringstream ss(line);
    std::string str_number;
    while (std::getline(ss, str_number, ','))
    {
        drawn.push_back(atoi(str_number.c_str()));
    }
    return drawn;
}

bool check_row(const board &b, const size_t b_i)
{
    for (size_t j = 0; j < b[b_i].size(); ++j)
    {
        if (!b[b_i][j].second)
        {
            return false;
        }
    }

    return true;
}

bool check_col(const board &b, const size_t b_j)
{
    for (size_t i = 0; i < b.size(); ++i)
    {
        if (!b[i][b_j].second)
        {
            return false;
        }
    }

    return true;
}

bool sing_number(const size_t n, board &b)
{
    for (size_t i = 0; i < b.size(); ++i)
    {
        for (size_t j = 0; j < b[i].size(); ++j)
        {
            auto &[board_number, check] = b[i][j];
            if (board_number == n)
            {
                check = true;
                return check_row(b, i) || check_col(b, j);
            }
        }
    }
    return false;
}

size_t compute_score(const board &b, size_t n)
{
    size_t sum{0};
    for (const auto &r : b)
    {
        for (const auto &[v, checked] : r)
        {
            if (!checked)
            {
                sum += v;
            }
        }
    }
    return sum * n;
}

void part_1()
{
    std::cout << "==========>PART 1<===========" << std::endl;
    const auto start = std::chrono::high_resolution_clock::now();
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return;
    }
    // https://en.wikipedia.org/wiki/CEIP_San_Ildefonso
    std::vector<size_t> ildephonse = read_drawn(input);
    std::vector<board> boards = read_boards(input);
    input.close();

    for (const auto &n : ildephonse)
    {
        for (auto &b : boards)
        {
            if (sing_number(n, b))
            {
                size_t res = compute_score(b, n);
                const auto stop = std::chrono::high_resolution_clock::now();
                const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                std::cout << "Part 1. Score is: " << res << '\n';
                std::cout << "It took " << duration.count() << "us \n";
                return;
            }
        }
    }
}

void part_2()
{
    std::cout << "==========>PART 2<===========" << std::endl;
    const auto start = std::chrono::high_resolution_clock::now();

    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        return;
    }
    // https://en.wikipedia.org/wiki/CEIP_San_Ildefonso
    std::vector<size_t> ildephonse = read_drawn(input);
    // just to reuse without wasting time refactoring.
    std::vector<std::pair<board, bool>> boards;
    std::vector<board> tmp_boards = read_boards(input);
    for (auto &&b : tmp_boards)
    {
        boards.push_back({std::move(b), false});
    }
    input.close();

    size_t already_won{0};
    for (const auto &n : ildephonse)
    {
        for (auto &[b, won] : boards)
        {
            if (!won && sing_number(n, b))
            {
                won = true;
                ++already_won;
                if (boards.size() == already_won)
                {
                    size_t res = compute_score(b, n);
                    const auto stop = std::chrono::high_resolution_clock::now();
                    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                    std::cout << "Part 2 score is: " << res << "\n";
                    std::cout << "It took " << duration.count() << "us \n";
                    return;
                }
            }
        }
    }
}

int main()
{
    part_1();
    part_2();
    return 0;
}