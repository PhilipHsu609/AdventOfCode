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

        auto parts = utils::split(line, " ");

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
            std::vector<int> button = utils::split<int>(button_str, ",");
            buttons.push_back(button);
        }
        machine.buttons = buttons;

        // Last part is joltage list
        auto joltage_str = parts.back().substr(1, parts.back().size() - 2);
        machine.joltage = utils::split<int>(joltage_str, ",");

        machines.push_back(machine);
    }

    return machines;
}

// Helper functions here

// A large value for 'infinity' cost, but safe from overflow when adding
const int INF = std::numeric_limits<int>::max() / 2;

// Solves integer linear systems Ax = b with non-negative constraints
// Uses Gaussian elimination + backtracking for free variables
struct IntegerSolver {
    int rows, cols;
    std::vector<std::vector<int>> A; // Augmented matrix [A|b]
    std::vector<int> best_solution;
    int min_total_presses = INF;

    IntegerSolver(const std::vector<std::vector<int>> &matrix) : A(matrix) {
        rows = static_cast<int>(A.size());
        cols = static_cast<int>(A[0].size()) - 1; // Last column is b
    }

    // Convert to row echelon form preserving integer arithmetic
    void reduce() {
        int pivot_row = 0;
        for (int col = 0; col < cols && pivot_row < rows; ++col) {
            // Find non-zero pivot
            int r = pivot_row;
            while (r < rows && A[r][col] == 0) {
                ++r;
            }

            if (r == rows) {
                continue;
            }

            std::swap(A[pivot_row], A[r]);

            // Eliminate below using: row[rr] = row[rr]*pivot - row[pivot]*A[rr][col]
            // This avoids division and preserves integer precision
            for (int rr = pivot_row + 1; rr < rows; ++rr) {
                if (A[rr][col] == 0) {
                    continue;
                }

                int mult_rr = A[pivot_row][col];
                int mult_pivot = A[rr][col];

                for (int c = col; c <= cols; ++c) {
                    A[rr][c] = A[rr][c] * mult_rr - A[pivot_row][c] * mult_pivot;
                }
            }
            ++pivot_row;
        }
    }

    // Back-substitution with backtracking for free variables
    void solve_recursive(int row_idx, int current_presses, std::vector<int> &solution) {
        if (current_presses >= min_total_presses) {
            return;
        }

        if (row_idx < 0) {
            min_total_presses = current_presses;
            best_solution = solution;
            return;
        }

        // Find pivot column (first non-zero coefficient)
        int pivot_col = -1;
        for (int j = 0; j < cols; ++j) {
            if (A[row_idx][j] != 0) {
                pivot_col = j;
                break;
            }
        }

        // Zero row: check consistency (0 = 0 is valid, 0 = non-zero is inconsistent)
        if (pivot_col == -1) {
            if (A[row_idx][cols] != 0) {
                return;
            }
            solve_recursive(row_idx - 1, current_presses, solution);
            return;
        }

        // Check for unassigned free variables to the right
        int first_free = -1;
        for (int j = cols - 1; j > pivot_col; --j) {
            if (solution[j] == -1) {
                first_free = j;
                break;
            }
        }

        // If free variable exists, enumerate values
        if (first_free != -1) {
            for (int val = 0; val < 1000; ++val) {
                solution[first_free] = val;
                if (current_presses + val < min_total_presses) {
                    solve_recursive(row_idx, current_presses + val, solution);
                }
                solution[first_free] = -1;
            }
            return;
        }

        // Solve for pivot variable: x[pivot] = (b - sum(A[j]*x[j])) / A[pivot]
        int sum_known = 0;
        for (int j = pivot_col + 1; j < cols; ++j) {
            sum_known += A[row_idx][j] * solution[j];
        }

        int remainder = A[row_idx][cols] - sum_known;
        int pivot_coeff = A[row_idx][pivot_col];

        // Check integer divisibility and non-negativity
        if (remainder % pivot_coeff != 0 || remainder / pivot_coeff < 0) {
            return;
        }

        int x_val = remainder / pivot_coeff;
        solution[pivot_col] = x_val;
        solve_recursive(row_idx - 1, current_presses + x_val, solution);
        solution[pivot_col] = -1;
    }

    int solve() {
        reduce();
        std::vector<int> current_sol(cols, -1);
        solve_recursive(rows - 1, 0, current_sol);
        return (min_total_presses == INF) ? 0 : min_total_presses;
    }
};

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

        IntegerSolver solver(M);
        ret += solver.solve();
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