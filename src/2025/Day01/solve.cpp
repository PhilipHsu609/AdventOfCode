#include "utils.hpp"

#include <fmt/core.h>

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

struct Rotate {
    char direction;
    int amount;
};

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);
    std::vector<Rotate> input;
    for (const auto &line : lines) {
        Rotate r{};
        r.direction = line[0];
        r.amount = std::stoi(line.substr(1));
        input.push_back(r);
    }
    return input;
}

// Helper functions here

template <typename T>
auto part1(const T &input) {
    int position = 50;
    int mod = 100;
    int ret = 0;

    for (const auto &rotate : input) {
        if (rotate.direction == 'L') {
            position = ((position - rotate.amount) % mod + mod) % mod;
        } else if (rotate.direction == 'R') {
            position = (position + rotate.amount) % mod;
        }
        if (position == 0) {
            ret++;
        }
    }

    return ret;
}

template <typename T>
auto part2(const T &input) {
    int position = 50;
    int mod = 100;
    int ret = 0;

    for (const auto &rotate : input) {
        int crossings = 0;

        if (position == 0) {
            // Starting at 0, both directions work the same
            crossings = rotate.amount / mod;
        } else if (rotate.direction == 'L') {
            // Going backward from position > 0
            if (rotate.amount >= position) {
                crossings = 1 + (rotate.amount - position) / mod;
            }
        } else {
            // Going forward from position > 0
            crossings = (position + rotate.amount) / mod;
        }

        ret += crossings;

        // Update position
        if (rotate.direction == 'L') {
            position = ((position - rotate.amount % mod) % mod + mod) % mod;
        } else {
            position = (position + rotate.amount) % mod;
        }
    }

    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 3;
    const auto part2_example_ans = 6;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}