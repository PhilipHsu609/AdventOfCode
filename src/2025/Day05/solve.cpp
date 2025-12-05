#include "utils.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace fs = std::filesystem;

struct Range {
    long long min;
    long long max;
    bool contains(long long value) const { return value >= min && value <= max; }
};

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);
    auto it = lines.begin();

    // Fresh ingredient ID ranges
    // e.g., "3-5"
    std::vector<Range> ingredient_ranges;
    while (!(*it).empty()) {
        Range r{};
        auto delim_pos = (*it).find('-');
        r.min = std::stoll((*it).substr(0, delim_pos));
        r.max = std::stoll((*it).substr(delim_pos + 1));
        ingredient_ranges.push_back(r);
        ++it;
    }
    ++it;

    // Available ingredient IDs
    std::vector<long long> available_ingredient_ids;
    while (it != lines.end()) {
        available_ingredient_ids.push_back(std::stoll(*it));
        ++it;
    }

    return std::make_tuple(ingredient_ranges, available_ingredient_ids);
}

// Helper functions here

template <typename T>
auto part1(const T &input) {
    const auto &[ingredient_ranges, available_ingredient_ids] = input;
    int ret = 0;

    for (const auto &id : available_ingredient_ids) {
        for (const auto &range : ingredient_ranges) {
            if (range.contains(id)) {
                ++ret;
                break;
            }
        }
    }

    return ret;
}

template <typename T>
auto part2(const T &input) {
    auto [ingredient_ranges, _] = input;
    long long ret = 0;

    std::sort(ingredient_ranges.begin(), ingredient_ranges.end(),
              [](const Range &a, const Range &b) {
                  if (a.min == b.min) {
                      return a.max < b.max;
                  }
                  return a.min < b.min;
              });

    // Merge overlapping ranges
    std::vector<Range> merged_ranges;
    for (const auto &range : ingredient_ranges) {
        if (merged_ranges.empty() || merged_ranges.back().max < range.min) {
            merged_ranges.push_back(range);
        } else {
            merged_ranges.back().max = std::max(merged_ranges.back().max, range.max);
        }
    }

    for (const auto &range : merged_ranges) {
        ret += (range.max - range.min + 1);
    }

    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 3;
    const auto part2_example_ans = 14LL;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}