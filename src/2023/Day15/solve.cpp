#include "utils.hpp"
#include <fmt/core.h> // fmt::print

#include <array>         // std::array
#include <cassert>       // assert
#include <cstdlib>       // std::exit
#include <filesystem>    // std::filesystem
#include <fstream>       // std::ifstream
#include <list>          // std::list
#include <string>        // std::getline
#include <string_view>   // std::string_view
#include <unordered_map> // std::unordered_map

namespace fs = std::filesystem;

int HASH(std::string_view s) {
    int ret = 0;
    for (char c : s) {
        ret = ((ret + c) * 17) % 256;
    }
    return ret;
}

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    std::string line;
    std::getline(input, line);
    auto steps = utils::split<std::string>(line, ",");

    int sum = 0;
    for (const auto &step : steps) {
        sum += HASH(step);
    }

    return sum;
}

int part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    std::string line;
    std::getline(input, line);
    auto steps = utils::split<std::string>(line, ",");

    std::array<std::list<int>, 256> HASHMAP{};
    std::array<std::unordered_map<std::string, std::list<int>::iterator>, 256> LABELS{};

    for (const auto &step : steps) {
        int idx = 0;
        while (step[idx] != '=' && step[idx] != '-') {
            idx++;
        }

        std::string label = step.substr(0, idx);
        int hash = HASH(label);
        char op = step[idx];

        if (op == '=') {
            int len = std::stoi(step.substr(idx + 1));

            if (LABELS[hash].find(label) == LABELS[hash].end()) {
                HASHMAP[hash].push_front(len);
                LABELS[hash][label] = HASHMAP[hash].begin();
            } else {
                auto it = LABELS[hash][label];
                *it = len;
            }
        } else {
            if (LABELS[hash].find(label) != LABELS[hash].end()) {
                auto it = LABELS[hash][label];
                HASHMAP[hash].erase(it);
                LABELS[hash].erase(label);
            }
        }
    }

    int sum = 0;
    for (size_t i = 0; i < HASHMAP.size(); i++) {
        int slot = 1;
        while (!HASHMAP[i].empty()) {
            sum += static_cast<int>(i + 1) * slot * HASHMAP[i].back();
            HASHMAP[i].pop_back();
            slot++;
        }
    }

    return sum;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 1320;
    const int part2_example_ans = 145;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}