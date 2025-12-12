#include "utils.hpp"

#include <fmt/core.h>

#include <cstdlib>
#include <filesystem>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);

    std::unordered_map<std::string, std::vector<std::string>> adj;
    for (const auto &line : lines) {
        // ccc: ddd eee fff
        auto parts = utils::split(line, ": ");
        auto node = parts[0];
        auto neighbors = utils::split(parts[1], " ");
        adj[node] = neighbors;
    }

    return adj;
}

// Helper functions here

template <typename T>
auto part1(const T &input) {
    int ret = 0;
    std::queue<std::string> q;
    q.emplace("you");

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (node == "out") {
            ++ret;
            continue;
        }

        for (const auto &neighbor : input.at(node)) {
            q.emplace(neighbor);
        }
    }

    return ret;
}

long long countPaths(const std::unordered_map<std::string, std::vector<std::string>> &adj,
                     const std::string &node, const std::string &dest,
                     std::unordered_map<std::string, long long> &memo) {
    if (node == dest) {
        return 1;
    }
    if (memo.find(node) != memo.end()) {
        return memo[node];
    }
    if (adj.find(node) == adj.end()) {
        return 0;
    }

    long long total = 0;
    for (const auto &neighbor : adj.at(node)) {
        total += countPaths(adj, neighbor, dest, memo);
    }
    memo[node] = total;
    return total;
}

template <typename T>
auto part2(T input) {
    long long ret = 0;

    auto memo = std::unordered_map<std::string, long long>{};
    auto svr_to_fft = countPaths(input, "svr", "fft", memo);
    memo.clear();
    auto fft_to_dac = countPaths(input, "fft", "dac", memo);
    memo.clear();
    auto dac_to_out = countPaths(input, "dac", "out", memo);
    ret += svr_to_fft * fft_to_dac * dac_to_out;

    memo.clear();
    auto svr_to_dac = countPaths(input, "svr", "dac", memo);
    memo.clear();
    auto dac_to_fft = countPaths(input, "dac", "fft", memo);
    memo.clear();
    auto fft_to_out = countPaths(input, "fft", "out", memo);
    ret += svr_to_dac * dac_to_fft * fft_to_out;

    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example1 = read_input("./example1.txt");
    const auto example2 = read_input("./example2.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 5;
    const auto part2_example_ans = 2LL;

    utils::assert_eq(part1(example1), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example2), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}