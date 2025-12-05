#include "utils.hpp"

#include <fmt/core.h>

#include <cstdlib>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) { return utils::read_lines(path); }

// Helper functions here

template <typename T>
int convolution(const T &input, int i, int j) {
    auto m = static_cast<int>(input.size());
    auto n = static_cast<int>(input[0].size());
    int ret = 0;

    for (int k = -1; k <= 1; ++k) {
        for (int l = -1; l <= 1; ++l) {
            int ii = i + k;
            int jj = j + l;

            if (ii < 0 || ii >= m || jj < 0 || jj >= n || (k == 0 && l == 0)) {
                continue;
            }

            if (input[ii][jj] == '@') {
                ++ret;
            }
        }
    }

    return ret;
}

template <typename T>
auto part1(const T &input) {
    auto m = static_cast<int>(input.size());
    auto n = static_cast<int>(input[0].size());
    int ret = 0;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (input[i][j] != '@') {
                continue;
            }
            ret += static_cast<int>(convolution(input, i, j) < 4);
        }
    }

    return ret;
}

template <typename T>
auto part2(const T &input) {
    auto grid = input;
    auto m = static_cast<int>(input.size());
    auto n = static_cast<int>(input[0].size());
    int ret = 0;

    int last_count = -1;

    while (true) {
        int current_count = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] != '@') {
                    continue;
                }
                ++current_count;
                int roll = convolution(grid, i, j);
                if (roll < 4) {
                    ++ret;
                    grid[i][j] = '.';
                    --current_count;
                }
            }
        }
        if (current_count == last_count) {
            break;
        }
        last_count = current_count;
    }

    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 13;
    const auto part2_example_ans = 43;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}