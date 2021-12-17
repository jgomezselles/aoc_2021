#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

// constexpr int64_t min_x = 20, max_x = 30, min_y = -10, max_y = -5;
constexpr int64_t min_x = 70, max_x = 125, min_y = -159, max_y = -121;
// target area: x=70..125, y=-159..-121

bool hit(const int64_t x, const int64_t y)
{
    return x >= min_x && x <= max_x && y >= min_y && y <= max_y;
}

bool launch(int64_t &res, int64_t vx, int64_t vy)
{
    int64_t x = 0, y = 0;
    while (x <= max_x && y >= min_y)
    {
        x += vx;
        y += vy;
        res = std::max(y, res);
        if (hit(x, y))
        {
            return true;
        }
        vx = vx > 0 ? vx - 1 : vx;
        --vy;
    }

    return false;
}

void solve()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    int64_t max_height{0};
    size_t n_shots{0};

    for (int64_t vx0 = 0; vx0 <= max_x; vx0++)
    {
        for (int64_t vy0 = min_y; vy0 <= -min_y; vy0++)
        {
            int64_t res{0};
            if (launch(res, vx0, vy0))
            {
                max_height = std::max(max_height, res);
                ++n_shots;
            }
        }
    }

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << max_height << '\n';
    std::cout << "Part 2: " << n_shots << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

int main()
{
    solve();
    return 0;
}