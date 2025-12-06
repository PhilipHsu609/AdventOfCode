#include "utils.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace fs = std::filesystem;

// For part 1: parse as 2D grid of integers with operations on last line
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);
    auto operations_line = lines.back();
    lines.pop_back();

    // Parse numeric grid
    std::vector<std::vector<int>> grid;
    for (const auto &line : lines) {
        std::istringstream iss(line);
        std::vector<int> row;
        int num{};
        while (iss >> num) {
            row.push_back(num);
        }
        grid.push_back(row);
    }

    // Parse operations
    std::istringstream iss(operations_line);
    std::vector<char> operations;
    char op{};
    while (iss >> op) {
        operations.push_back(op);
    }

    return std::make_tuple(grid, operations);
}

// For part 2: raw lines to read vertically
auto read_input_raw(const fs::path &path) { return utils::read_lines(path); }

// Helper functions for part 2
std::string read_column(const std::vector<std::string> &grid, size_t col_idx) {
    std::string column;
    for (const auto &row : grid) {
        column += row[col_idx];
    }
    return column;
}

bool is_separator_column(const std::string &col) {
    return std::all_of(col.begin(), col.end(), [](char c) { return c == ' '; });
}

template <typename T>
auto part1(const T &input) {
    auto [grid, operations] = input;

    auto num_rows = grid.size();
    auto num_cols = grid[0].size();

    if (num_cols != operations.size()) {
        return -1LL;
    }

    long long total = 0;

    // Process each column with its operation
    for (size_t col_idx = 0; col_idx < num_cols; ++col_idx) {
        char op = operations[col_idx];
        long long column_result = (op == '+') ? 0 : 1;

        for (size_t row_idx = 0; row_idx < num_rows; ++row_idx) {
            if (op == '+') {
                column_result += grid[row_idx][col_idx];
            } else { // '*'
                column_result *= grid[row_idx][col_idx];
            }
        }
        total += column_result;
    }

    return total;
}

template <typename T>
auto part2(const T &input) {
    auto num_cols = input[0].size();
    long long total = 0;

    char current_op = '+'; // Default operator
    std::vector<long long> number_group;

    for (size_t col_idx = 0; col_idx < num_cols; ++col_idx) {
        std::string column = read_column(input, col_idx);

        // Extract operator if present (last character in column)
        if (column.back() == '*' || column.back() == '+') {
            current_op = column.back();
            column.pop_back();
        }

        // Check if this is a separator or the last column
        bool is_separator = is_separator_column(column);
        bool is_last = (col_idx == num_cols - 1);

        if (!is_separator) {
            number_group.push_back(std::stoll(column));
        }

        // Calculate result when we hit a separator or end
        if (is_separator || is_last) {
            if (current_op == '+') {
                total += std::accumulate(number_group.begin(), number_group.end(), 0LL);
            } else { // '*'
                total += std::accumulate(number_group.begin(), number_group.end(), 1LL,
                                         std::multiplies<long long>());
            }
            number_group.clear();
        }
    }

    return total;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example1 = read_input("./example.txt");
    const auto example2 = read_input_raw("./example.txt");
    const auto input1 = read_input("./input.txt");
    const auto input2 = read_input_raw("./input.txt");
    const auto part1_example_ans = 4277556LL;
    const auto part2_example_ans = 3263827LL;

    utils::assert_eq(part1(example1), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example2), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input1);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input2);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}