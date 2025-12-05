#include "utils.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <string>

namespace fs = std::filesystem;

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) { return utils::read_lines(path); }

// Helper functions here

template <typename T>
auto part1(const T &input) {
    int ret = 0;
    for (const auto &line : input) {
        int joltage = 0;
        int max_b = -1;
        std::for_each(line.rbegin(), line.rend(), [&](char c) {
            int b = c - '0';
            if (max_b != -1) {
                joltage = std::max(joltage, b * 10 + max_b);
            }
            max_b = std::max(max_b, b);
        });
        ret += joltage;
    }
    return ret;
}

template <typename T>
auto part2(const T &input) {
    constexpr int target_len = 12;
    long long ret = 0LL;

    for (const auto &line : input) {
        long long joltage = 0LL;

        size_t start = 0;
        size_t end = line.size() - target_len + 1;
        for (int i = 1; i <= target_len; ++i) {
            auto d = std::max_element(line.begin() + start, line.begin() + end);

            start = std::distance(line.begin(), d) + 1;
            end = line.size() - target_len + (i + 1);

            if (start >= end) {
                end = start + 1;
            }

            joltage = joltage * 10 + (*d - '0');
        }

        ret += joltage;
    }
    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 357;
    const auto part2_example_ans = 3121910778619LL;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}