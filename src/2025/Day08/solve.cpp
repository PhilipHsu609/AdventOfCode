#include "disjoint_set.hpp"
#include "utils.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

struct Point {
    int x;
    int y;
    int z;
};

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);
    std::vector<Point> coords;
    coords.reserve(lines.size());
    for (const auto &line : lines) {
        auto values = utils::split<int>(line, ',');
        coords.emplace_back(Point{values[0], values[1], values[2]});
    }
    return coords;
}

// Helper functions here

template <typename T>
auto part1(const T &input, size_t connections) {
    size_t n = input.size();

    // {distance, point1_index, point2_index}
    std::vector<std::array<size_t, 3>> edges;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            size_t dist = std::pow(input[i].x - input[j].x, 2) +
                          std::pow(input[i].y - input[j].y, 2) +
                          std::pow(input[i].z - input[j].z, 2);
            edges.push_back({dist, i, j});
        }
    }

    std::sort(edges.begin(), edges.end());

    DisjointSet ds(n);
    for (const auto &[dist, u, v] : edges) {
        ds.unite(u, v);
        if (--connections == 0) {
            break;
        }
    }

    // Get the top 3 largest components
    std::vector<size_t> component_sizes(n, 0);
    for (size_t i = 0; i < n; ++i) {
        component_sizes[ds.find(i)]++;
    }

    std::sort(component_sizes.begin(), component_sizes.end(), std::greater<size_t>());
    size_t result = component_sizes[0] * component_sizes[1] * component_sizes[2];

    return result;
}

template <typename T>
auto part2(const T &input) {
    size_t n = input.size();
    size_t components = n;

    // {distance, point1_index, point2_index}
    std::vector<std::array<size_t, 3>> edges;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            size_t dist = std::pow(input[i].x - input[j].x, 2) +
                          std::pow(input[i].y - input[j].y, 2) +
                          std::pow(input[i].z - input[j].z, 2);
            edges.push_back({dist, i, j});
        }
    }

    std::sort(edges.begin(), edges.end());

    size_t result = 0;
    DisjointSet ds(n);
    for (const auto &[dist, u, v] : edges) {
        if (ds.find(u) != ds.find(v)) {
            ds.unite(u, v);
            components--;
        }

        if (components == 1) {
            result = static_cast<size_t>(input[u].x) * static_cast<size_t>(input[v].x);
            break;
        }
    }

    return result;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 40uL;
    const auto part2_example_ans = 25272uL;

    utils::assert_eq(part1(example, 10), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input, 1000);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}