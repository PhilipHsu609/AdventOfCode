#include "utils.hpp"

#include <fmt/core.h>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <tuple>
#include <vector>

namespace fs = std::filesystem;

struct Present {
    int id;
    int size;
};

struct Region {
    std::array<int, 2> shape;
    std::vector<int> requirements;
};

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);

    std::tuple<std::vector<Present>, std::vector<Region>> data;
    std::vector<Present> presents;
    std::vector<Region> regions;

    for (size_t i = 0; i < 6; ++i) {
        Present p{};
        p.id = static_cast<int>(i);
        for (size_t j = 0; j < 3; ++j) {
            for (char c : lines[i * 5 + j + 1]) {
                if (c == '#') {
                    p.size++;
                }
            }
        }
        presents.push_back(p);
    }

    for (size_t i = 30; i < lines.size(); ++i) {
        Region r{};
        auto parts = utils::split(lines[i], ": ");

        auto shape_parts = utils::split(parts[0], "x");
        r.shape = {std::stoi(shape_parts[0]), std::stoi(shape_parts[1])};
        r.requirements = utils::split<int>(parts[1], " ");
        regions.push_back(r);
    }

    return std::make_tuple(presents, regions);
}

// Helper functions here

template <typename T>
auto part1(const T &input) {
    const auto &[presents, regions] = input;

    int ret = 0;
    for (const auto &r : regions) {
        // Assume each present is 3x3 and check if it fits
        int total = 0;
        for (const auto &num : r.requirements) {
            total += num * 9;
        }

        if (total <= r.shape[0] * r.shape[1]) {
            ret++;
        }
    }

    return ret;
}

template <typename T>
auto part2(const T &input) {
    return 0;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 1;
    const auto part2_example_ans = 0;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}