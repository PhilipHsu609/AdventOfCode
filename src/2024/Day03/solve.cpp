#include "utils.hpp"

#include <fmt/core.h>

#include <cstdlib>
#include <filesystem>
#include <regex>
#include <string>

namespace fs = std::filesystem;

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) { return utils::read_lines(path); }

// Helper functions here

template <typename T>
auto part1(const T &input) {
    int ret = 0;
    std::regex re(R"(mul\((\d+),(\d+)\))");

    for (auto mem : input) {
        std::smatch match;
        while (std::regex_search(mem, match, re)) {
            ret += std::stoi(match[1]) * std::stoi(match[2]);
            mem = match.suffix().str();
        }
    }

    return ret;
}

template <typename T>
auto part2(const T &input) {
    int ret = 0;
    // Combined regex: matches mul(a,b) or don't() or do()
    std::regex re(R"((mul\((\d+),(\d+)\))|(don't\(\))|(do\(\)))");
    bool enabled = true;

    for (auto mem : input) {
        std::smatch match;
        while (std::regex_search(mem, match, re)) {
            auto op = match[0].str();

            if (enabled && op[0] == 'm') {
                ret += std::stoi(match[2]) * std::stoi(match[3]);
            } else if (op == "do()") {
                enabled = true;
            } else if (op == "don't()") {
                enabled = false;
            }

            mem = match.suffix().str();
        }
    }

    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 161;
    const auto part2_example_ans = 48;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}