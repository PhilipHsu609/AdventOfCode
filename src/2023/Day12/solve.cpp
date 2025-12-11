#include "utils.hpp" // split

#include <fmt/core.h> // print

#include <cassert>     // assert
#include <cstdlib>     // exit
#include <filesystem>  // path, current_path, absolute
#include <fstream>     // ifstream
#include <numeric>     // accumulate
#include <string>      // string, getline
#include <string_view> // string_view
#include <vector>      // vector

namespace fs = std::filesystem;

using Memo = std::vector<std::vector<long long>>;

int check_valid(std::string_view pattern, const std::vector<int> &record) {
    // check if the pattern is valid
    const int n = static_cast<int>(pattern.size());
    const int m = static_cast<int>(record.size());
    int p_count = 0;
    int p_idx = 0;
    int r_idx = 0;

    while (p_idx < n) {
        while (p_idx < n && pattern[p_idx] == '#') {
            p_idx++;
            p_count++;
        }

        if (p_count > 0) {
            if (r_idx == m || p_count != record[r_idx]) {
                return 0;
            }
            r_idx++;
        }

        p_count = 0;
        p_idx++;
    }

    return r_idx == m ? 1 : 0;
}

template <typename T>
T dup(T container, int n) {
    T result;
    for (int i = 0; i < n; i++) {
        result.insert(result.end(), container.begin(), container.end());
    }
    return result;
}

// brute force solver
[[maybe_unused]]
long long bs_solver(int idx, std::string pattern, const std::vector<int> &records) {
    const int n = static_cast<int>(pattern.size());

    if (idx == n) {
        return check_valid(pattern, records);
    }

    long long ans = 0;
    if (pattern[idx] == '?') {
        // unknown spring '?'
        pattern[idx] = '#'; // set to damaged
        ans += bs_solver(idx + 1, pattern, records);
        pattern[idx] = '.'; // set to operational
        ans += bs_solver(idx + 1, pattern, records);
    } else {
        ans += bs_solver(idx + 1, pattern, records);
    }

    return ans;
}

long long dp_solver(int i, int j, std::string_view pattern,
                    const std::vector<int> &records, Memo &memo) {
    /*
    Reference:
    - https://github.com/hb0nes/aoc_2023/blob/main/twelve_dp/src/main.rs)
    Interesing solutions:
    - https://github.com/AlexOxorn/AdventOfCode/blob/main/puzzles/2023/src/day12.cpp
    - https://github.com/maneatingape/advent-of-code-rust/blob/main/src/year2023/day12.rs
    */
    const int n = static_cast<int>(pattern.size());
    const int m = static_cast<int>(records.size());

    if (j == m) {
        /*
            Base case
            - all records are matched
            - the rest of the pattern should not contain '#'
        */
        return pattern.substr(i).find('#') == std::string::npos ? 1 : 0; // NOLINT
    }

    if (memo[i][j] != -1) {
        return memo[i][j];
    }

    // The minimum length of the pattern that needs to be matched
    const int min_len =
        std::accumulate(records.begin() + j, records.end(), 0) + (m - j) - 1;
    if ((n - i) < min_len) {
        // pattern[i:] is too short to match the remaining records
        return 0;
    }

    if (pattern[i] == '.') {
        // if the current spring is functional, skip it
        return dp_solver(i + 1, j, pattern, records, memo);
    }

    const int record = records[j];
    long long ans = 0;

    // check if pattern[i:i+record] contains only '#' or '?'
    const bool cond1 = pattern.substr(i, record).find('.') == std::string::npos; // NOLINT

    // check if pattern[i+record] is not '#'
    // because we need a '.' to separate the damaged springs
    const bool cond2 = i + record == n || pattern[i + record] != '#';

    if (cond1 && cond2) {
        // if two conditions are satisfied, then we advance to the next record
        ans += dp_solver(i + record + 1, j + 1, pattern, records, memo);
    }

    if (pattern[i] == '?') {
        // In the previous if statement, we treat every '?' as a damaged spring '#'
        // now we treat the first '?' as a functional spring '.'
        ans += dp_solver(i + 1, j, pattern, records, memo);
    }

    return memo[i][j] = ans;
}

long long part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    long long ans = 0;
    std::string line;
    while (std::getline(input, line)) {
        auto parts = utils::split(line, " ");
        const std::string pattern = parts[0] + '.'; // add a trailing '.' for convenience
        const std::vector<int> records = utils::split<int>(parts[1], ",");

        Memo memo(pattern.size() + 1, std::vector<long long>(records.size() + 1, -1));
        ans += dp_solver(0, 0, pattern, records, memo);
    }

    return ans;
}

long long part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    long long ans = 0;
    std::string line;
    while (std::getline(input, line)) {
        auto parts = utils::split(line, " ");
        std::string pattern = parts[0] + '?';
        std::vector<int> records = utils::split<int>(parts[1], ",");

        // duplicate the pattern and records
        pattern = dup(pattern, 5);
        records = dup(records, 5);
        pattern.back() = '.';

        Memo memo(pattern.size() + 1, std::vector<long long>(records.size() + 1, -1));
        ans += dp_solver(0, 0, pattern, records, memo);
    }

    return ans;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const long long part1_example_ans = 21LL;
    const long long part2_example_ans = 525152LL;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}