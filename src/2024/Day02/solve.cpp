#include "utils.hpp" // utils::split

#include <fmt/core.h> // fmt::print

#include <algorithm>  // std::is_sorted
#include <cassert>    // assert
#include <cstdlib>    // std::exit
#include <filesystem> // std::filesystem
#include <fstream>    // std::ifstream
#include <limits>     // std::numeric_limits
#include <string>     // std::getline
#include <vector>     // std::vector

namespace fs = std::filesystem;

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    int safe = 0;

    std::string line;
    while (std::getline(input, line)) {
        auto report = utils::split<int>(line, ' ');

        bool inc = true;
        bool dec = true;
        int max_diff = 0;
        int min_diff = std::numeric_limits<int>::max();

        for (size_t i = 1; i < report.size(); i++) {
            if (report[i] < report[i - 1]) {
                inc = false;
            }
            if (report[i] > report[i - 1]) {
                dec = false;
            }
            int diff = std::abs(report[i] - report[i - 1]);
            max_diff = std::max(max_diff, diff);
            min_diff = std::min(min_diff, diff);
        }

        if ((inc || dec) && max_diff <= 3 && min_diff >= 1) {
            safe++;
        }
    }

    return safe;
}

int try_remove(const std::vector<int> &report) {
    for (size_t i = 0; i < report.size(); i++) {
        std::vector<int> copy = report;
        copy.erase(copy.begin() + static_cast<long>(i));

        bool inc = true;
        bool dec = true;
        int max_diff = 0;
        int min_diff = std::numeric_limits<int>::max();

        for (size_t j = 1; j < copy.size(); j++) {
            if (copy[j] < copy[j - 1]) {
                inc = false;
            }
            if (copy[j] > copy[j - 1]) {
                dec = false;
            }
            int diff = std::abs(copy[j] - copy[j - 1]);
            max_diff = std::max(max_diff, diff);
            min_diff = std::min(min_diff, diff);
        }

        if ((inc || dec) && max_diff <= 3 && min_diff >= 1) {
            return 1;
        }
    }

    return 0;
}

int part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    int safe = 0;

    std::string line;
    while (std::getline(input, line)) {
        auto report = utils::split<int>(line, ' ');

        bool inc = true;
        bool dec = true;
        int max_diff = 0;
        int min_diff = std::numeric_limits<int>::max();

        for (size_t i = 1; i < report.size(); i++) {
            if (report[i] < report[i - 1]) {
                inc = false;
            }
            if (report[i] > report[i - 1]) {
                dec = false;
            }
            int diff = std::abs(report[i] - report[i - 1]);
            max_diff = std::max(max_diff, diff);
            min_diff = std::min(min_diff, diff);
        }

        if ((inc || dec) && max_diff <= 3 && min_diff >= 1) {
            safe++;
        } else {
            safe += try_remove(report);
        }
    }

    return safe;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 2;
    const int part2_example_ans = 4;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}