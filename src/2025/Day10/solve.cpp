#include "utils.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <cstdlib>
#include <filesystem>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

struct Machine {
    unsigned target_state;
    std::vector<std::vector<int>> buttons;
    std::vector<int> joltage;
};

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);
    std::vector<Machine> machines;

    for (const auto &line : lines) {
        // Line format example:
        // [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
        Machine machine{};

        auto parts = utils::split(line, ' ');

        // First part is target state
        auto target_str = parts[0].substr(1, parts[0].size() - 2);
        unsigned target = 0;
        for (size_t i = 0; i < target_str.size(); ++i) {
            if (target_str[i] == '#') {
                target |= (1 << i);
            }
        }
        machine.target_state = target;

        // Subsequent parts are button definitions except the last one
        std::vector<std::vector<int>> buttons;
        for (size_t i = 1; i < parts.size() - 1; ++i) {
            auto button_str = parts[i].substr(1, parts[i].size() - 2);
            std::vector<int> button = utils::split<int>(button_str, ',');
            buttons.push_back(button);
        }
        machine.buttons = buttons;

        // Last part is joltage list
        auto joltage_str = parts.back().substr(1, parts.back().size() - 2);
        machine.joltage = utils::split<int>(joltage_str, ',');

        machines.push_back(machine);
    }

    return machines;
}

// Helper functions here

template <typename T>
auto part1(const T &input) {
    int ret = 0;

    for (const auto &machine : input) {
        std::queue<std::pair<unsigned, int>> q; // (state, steps)
        std::unordered_set<unsigned> visited;
        q.emplace(0, 0);

        while (!q.empty()) {
            auto [state, steps] = q.front();
            q.pop();

            if (state == machine.target_state) {
                ret += steps;
                break;
            }

            visited.insert(state);

            for (const auto &button : machine.buttons) {
                auto new_state = state;
                for (int bit : button) {
                    new_state ^= (1 << bit);
                }

                if (visited.find(new_state) == visited.end()) {
                    q.emplace(new_state, steps + 1);
                }
            }
        }
    }

    return ret;
}

template <typename T>
auto part2(const T &input) {
    int ret = 0;

    for (const auto &machine : input) {
        // Construct Ax=b system and solve with Gauss-Jordan elimination
        // A \in N^{m x n} (but every element is 0 or 1)
        // x \in N^n
        // b \in N^m
        size_t m = machine.joltage.size();
        size_t n = machine.buttons.size();

        // Let M be the augmented matrix
        // M = [A|b]
        std::vector<std::vector<int>> M(m, std::vector<int>(n + 1, 0));
        for (size_t i = 0; i < n; ++i) {
            for (int idx : machine.buttons[i]) {
                M[idx][i] = 1;
            }
        }
        for (size_t i = 0; i < m; ++i) {
            M[i][n] = machine.joltage[i];
        }

        // Print initial matrix for debugging
        fmt::print("Initial Matrix:\n");
        for (const auto &row : M) {
            fmt::print("{}", row);
        }

        // Convert M to RREF
        size_t pivot_row = 0;
        std::vector<int> pivot_col_for_row(m, -1);
        for (size_t col = 0; col < n && pivot_row < m; ++col) {
            // Find the first row with a leading 1 in this column
            size_t r{};
            for (r = pivot_row; r < m; ++r) {
                if (M[r][col] != 0) {
                    break;
                }
            }

            std::swap(M[pivot_row], M[r]);
            pivot_col_for_row[pivot_row] = static_cast<int>(col);

            // Eliminate all other rows below
            for (size_t rr = pivot_row + 1; rr < m; ++rr) {
                for (size_t c = col; c <= n; ++c) {
                    M[rr][c] -= M[pivot_row][c] * M[rr][col];
                }
            }
            ++pivot_row;
        }

        // Print RREF for debugging
        for (const auto &row : M) {
            fmt::print("{}", row);
        }
    }

    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 7;
    const auto part2_example_ans = 33;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}