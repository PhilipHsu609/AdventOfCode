#include <fmt/core.h>
#include <fmt/ranges.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#include "../../Utils/utils.hpp"

namespace fs = std::filesystem;

void part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        return;
    }

    std::string line;
    long long result = 0LL;

    while (std::getline(input, line)) {
        std::vector<int> values = Utils::split<int>(line, ' ');
        int n = static_cast<int>(values.size());

        while (n > 2) {
            for (int i = 0; i < n - 1; i++) {
                values[i] = values[i + 1] - values[i];
            }
            --n;
        }

        result += std::accumulate(values.begin(), values.end(), 0LL);
    }

    fmt::print("Part 1: {}\n", result);
}

void part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        return;
    }

    std::string line;
    long long result = 0LL;

    while (std::getline(input, line)) {
        std::vector<int> values = Utils::split<int>(line, ' ');
        const int n = static_cast<int>(values.size());
        int start = 0;

        while (start < n) {
            for (int i = n - 1; i > start; i--) {
                values[i] = values[i] - values[i - 1];
            }
            ++start;
        }

        long long value = values.back();
        for (int i = n - 2; i >= 0; i--) {
            value = values[i] - value;
        }

        result += value;
    }

    fmt::print("Part 2: {}\n", result);
}

int main() {
    const fs::path input_path = "./input.txt";
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1(input_path);
    part2(input_path);
    return 0;
}