#include "utils.hpp"

#include <fmt/core.h>

#include <cstdlib>
#include <filesystem>
#include <numeric>
#include <string>

namespace fs = std::filesystem;

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) { return utils::read_lines(path); }

// Helper functions here

template <typename T>
auto part1(const T &input) {
    auto S = static_cast<int>(input[0].find('S'));
    auto m = static_cast<int>(input.size());
    auto n = static_cast<int>(input[0].size());

    std::string current_row(n, '.');
    current_row[S] = '|';

    int ret = 0;
    for (int i = 0; i < m - 1; ++i) {
        std::string next_row(n, '.');
        for (int j = 0; j < n; ++j) {
            if (current_row[j] == '|') {
                if (input[i + 1][j] == '^') {
                    next_row[j - 1] = '|';
                    next_row[j + 1] = '|';
                    ++ret;
                } else {
                    next_row[j] = '|';
                }
            }
        }
        current_row = next_row;
    }

    return ret;
}

template <typename T>
auto part2(const T &input) {
    auto S = static_cast<int>(input[0].find('S'));
    auto m = static_cast<int>(input.size());
    auto n = static_cast<int>(input[0].size());

    std::vector<long long> current_row(n, 0);
    current_row[S] = 1;

    for (int i = 0; i < m - 1; ++i) {
        std::vector<long long> next_row(n, 0);
        for (int j = 0; j < n; ++j) {
            if (current_row[j] > 0) {
                if (input[i + 1][j] == '^') {
                    next_row[j - 1] += current_row[j];
                    next_row[j + 1] += current_row[j];
                } else {
                    next_row[j] += current_row[j];
                }
            }
        }
        current_row = next_row;
    }

    return std::accumulate(current_row.begin(), current_row.end(), 0LL);
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 21;
    const auto part2_example_ans = 40LL;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}