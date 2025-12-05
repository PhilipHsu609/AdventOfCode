#include "utils.hpp"

#include <fmt/core.h>

#include <cstdlib>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) { return utils::read_lines(path); }

// Helper functions here

int searchXMAS(const std::vector<std::string> &input) {
    static constexpr std::string_view pattern = "XMAS";
    static constexpr std::string_view pattern_rev = "SAMX";

    auto m = static_cast<int>(input.size());
    auto n = static_cast<int>(input[0].size());

    int ret = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::string window;
            // Check horizontal
            window.clear();
            for (int k = 0; k < 4; ++k) {
                if (j - k < 0) {
                    break;
                }
                window += input[i][j - k];
            }
            ret += (window == pattern_rev) ? 1 : 0;
            ret += (window == pattern) ? 1 : 0;

            // Check vertical
            window.clear();
            for (int k = 0; k < 4; ++k) {
                if (i - k < 0) {
                    break;
                }
                window += input[i - k][j];
            }
            ret += (window == pattern_rev) ? 1 : 0;
            ret += (window == pattern) ? 1 : 0;

            // Check diagonal (top-left to bottom-right)
            window.clear();
            for (int k = 0; k < 4; ++k) {
                if (i - k < 0 || j - k < 0) {
                    break;
                }
                window += input[i - k][j - k];
            }
            ret += (window == pattern_rev) ? 1 : 0;
            ret += (window == pattern) ? 1 : 0;

            // Check diagonal (top-right to bottom-left)
            window.clear();
            for (int k = 0; k < 4; ++k) {
                if (i - k < 0 || j + k >= n) {
                    break;
                }
                window += input[i - k][j + k];
            }
            ret += (window == pattern_rev) ? 1 : 0;
            ret += (window == pattern) ? 1 : 0;
        }
    }
    return ret;
}

int searchX_MAS(const std::vector<std::string> &input) {
    static constexpr std::string_view pattern = "MAS";
    static constexpr std::string_view pattern_rev = "SAM";

    auto m = static_cast<int>(input.size());
    auto n = static_cast<int>(input[0].size());

    int ret = 0;
    for (int i = 1; i < m - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            // Check 3x3 grid centered at (i, j) for "MAS" or "SAM" cross
            std::string diag1;
            std::string diag2;
            diag1 += input[i - 1][j - 1];
            diag1 += input[i][j];
            diag1 += input[i + 1][j + 1];
            diag2 += input[i - 1][j + 1];
            diag2 += input[i][j];
            diag2 += input[i + 1][j - 1];
            ret += (diag1 == pattern || diag1 == pattern_rev) &&
                           (diag2 == pattern || diag2 == pattern_rev)
                       ? 1
                       : 0;
        }
    }
    return ret;
}

template <typename T>
auto part1(const T &input) {
    return searchXMAS(input);
}

template <typename T>
auto part2(const T &input) {
    return searchX_MAS(input);
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 18;
    const auto part2_example_ans = 9;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}