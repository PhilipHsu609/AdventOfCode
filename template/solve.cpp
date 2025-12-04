#include "utils.hpp"

#include <fmt/core.h>

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) { return utils::read_lines(path); }

// Helper functions here

template <typename T>
auto part1(const T &input) {
    return 0;
}

template <typename T>
auto part2(const T &input) {
    return 0;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 0;
    const auto part2_example_ans = 0;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}