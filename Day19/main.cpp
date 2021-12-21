#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <set>
#include <sstream>
#include <cmath>
#include <algorithm>

struct point
{
    point() = delete;
    point(const int64_t i, const int64_t j, const int64_t k) : x(i), y(j), z(k), dists(){};
    float distance(const point &other) const;
    void compute_inter_distances(const std::vector<point> &scanner);
    void transpose(const point &distances);

    bool operator==(const point &other) const { return std::tie(x, y, z) == std::tie(other.x, other.y, other.z); };
    bool operator>(const point &other) const { return std::tie(x, y, z) > std::tie(other.x, other.y, other.z); };
    bool operator<(const point &other) const { return std::tie(x, y, z) < std::tie(other.x, other.y, other.z); };

    void operator+=(const point &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
    };
    void operator-=(const point &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    };

    point operator-(const point &other) const
    {
        return point(x - other.x, y - other.y, z - other.z);
    };

    point operator+(const point &other) const
    {
        return point(x + other.x, y + other.y, z + other.z);
    };

    void create_candidates();

    friend std::ostream &operator<<(std::ostream &os, const point &p);

    int64_t x;
    int64_t y;
    int64_t z;
    std::set<float> dists;
    std::vector<point> candidates;
};

void point::create_candidates()
{
    candidates = {{x, y, z}, {x, -z, y}, {x, -y, -z}, {x, z, -y}, {y, z, x}, {y, -x, z}, {y, -z, -x}, {y, x, -z}, {z, x, y}, {z, -y, x}, {z, -x, -y}, {z, y, -x}, {-z, -y, -x}, {-z, x, -y}, {-z, y, x}, {-z, -x, y}, {-y, -x, -z}, {-y, z, -x}, {-y, x, z}, {-y, -z, x}, {-x, -z, -y}, {-x, y, -z}, {-x, z, y}, {-x, -y, z}};
}

std::ostream &operator<<(std::ostream &os, const point &p)
{
    os << p.x << ',' << p.y << ',' << p.z;
    return os;
}

float point::distance(const point &other) const
{
    const int64_t dx = fabs(x - other.x), dy = fabs(y - other.y), dz = fabs(z - other.z);
    return sqrt(dx * dx + dy * dy + dz * dz);
}

void point::compute_inter_distances(const std::vector<point> &scanner)
{
    for (const auto &p : scanner)
    {
        dists.insert(distance(p));
    }
}

bool same_float(const float &first, const float &second)
{
    return fabs(first - second) < 0.001;
}

std::vector<std::vector<point>> read_input()
{
    std::ifstream input("input_test.txt");
    if (!input)
    {
        std::cerr << "Cannot open the input.txt" << std::endl;
        exit(1);
    }
    std::string line;
    std::vector<std::vector<point>> scanner_reads;
    std::vector<point> scanner;
    bool new_scanner{true};
    while (getline(input, line))
    {
        if (line.empty())
        {
            new_scanner = true;
            continue;
        }

        if (new_scanner)
        {
            new_scanner = false;
            if (scanner.size())
            {
                for (auto &p : scanner)
                {
                    p.compute_inter_distances(scanner);
                    p.create_candidates();
                }
                scanner_reads.push_back(scanner);
                scanner.clear();
            }
            continue;
        }

        std::stringstream ss(line);
        std::string coord;
        getline(ss, coord, ',');
        int64_t i, j, k;
        i = stoi(coord);
        getline(ss, coord, ',');
        j = stoi(coord);
        getline(ss, coord);
        k = stoi(coord);
        scanner.push_back(point(i, j, k));
    }

    for (auto &p : scanner)
    {
        p.compute_inter_distances(scanner);
    }
    scanner_reads.push_back(scanner);
    scanner.clear();

    input.close();

    return scanner_reads;
}

size_t intersection_size(const std::set<float> &a, const std::set<float> &b)
{
    size_t inter{0};
    for (const auto &da : a)
    {
        if (auto it = b.lower_bound(da - 0.001); it != b.end() && same_float(da, *it))
        {
            ++inter;
        }
    }
    return inter;
}

void transpose_beacons(std::vector<point> &s, const point &trans_dist)
{
    for (auto &p : s)
    {
        // std::cout << "Transposing: " << p << '\n';
        // std::cout << "Distance: " << trans_dist << '\n';
        p += trans_dist;
        // std::cout << "Result: " << p << '\n';
    }
}

bool is_transposed(const size_t idx, const std::set<size_t> &transposed_scanners)
{
    return transposed_scanners.find(idx) != transposed_scanners.end();
}

void insert_beacons(const std::vector<point> &scanner, std::set<point> &beacons)
{
    for (const auto &p : scanner)
    {
        if (auto [it, b] = beacons.insert(p); !b)
        {
            // std::cout << "Already exists. Skipping inserting " << p << '\n';
        }
    }
}

std::pair<bool, point> overlap(const std::vector<point> &a, const std::vector<point> &b)
{
    for (const auto &pa : a)
    {
        for (const auto &pb : b)
        {
            if (size_t d = intersection_size(pa.dists, pb.dists); d >= 12)
            {
                auto p_dist = pb + pa;
                return {true, p_dist};
            }
        }
    }
    return {false, {0, 0, 0}};
}

size_t search_candidate(const std::vector<point> &ref_scanner, const std::vector<point> &scanner)
{
    for (const auto p : scanner)
    {
        for (size_t candidate = 0; candidate < p.candidates.size(); candidate++)
        {
            if(auto it = std::find(ref_scanner.begin(), ref_scanner.end(), p.candidates.at(candidate)); it != ref_scanner.end())
            {

            }
        }
    }
}

void transpose_scanner(const size_t ref_i, std::vector<std::vector<point>> &scanners, std::set<size_t> &transposed_scanners, std::set<point> &beacons)
{
    for (size_t i = 0; i < scanners.size(); i++)
    {
        if (!is_transposed(i, transposed_scanners))
        {
            if (const auto [b, dist] = overlap(scanners.at(ref_i), scanners.at(i)); b)
            {
                std::cout << ref_i << " and " << i << " overlap with dist=" << dist << '\n';
                size_t n = search_candidate(scanners.at(ref_i), scanners.at(i));
                transpose_beacons(scanners.at(i), dist);
                transposed_scanners.insert(i);
                insert_beacons(scanners.at(i), beacons);
                transpose_scanner(i, scanners, transposed_scanners, beacons);
            }
        }
    }
}

void part_1()
{
    const auto t_start = std::chrono::high_resolution_clock::now();
    auto scanners = read_input();
    std::set<size_t> transposed_scanners{0};
    std::set<point> beacons;
    insert_beacons(scanners[0], beacons);
    transpose_scanner(0, scanners, transposed_scanners, beacons);

    auto res = beacons.size();

    /*for (const auto &p : beacons)
    {
        std::cout << p << '\n';
    }
    std::cout << '\n';*/

    const auto t_stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start);

    std::cout << "Part 1: " << res << '\n';
    std::cout << "It took " << duration.count() << "us \n";
    return;
}

void part_2()
{
}

int main()
{
    part_1();
    part_2();
    return 0;
}