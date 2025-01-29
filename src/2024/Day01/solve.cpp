#include <fmt/core.h> // fmt::print

#include <algorithm>     // std::sort
#include <cassert>       // assert
#include <cstdlib>       // std::exit
#include <filesystem>    // std::filesystem
#include <fstream>       // std::ifstream
#include <sstream>       // std::stringstream
#include <string>        // std::getline
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

namespace fs = std::filesystem;

long long part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    std::vector<int> l_list;
    std::vector<int> r_list;

    int lines = 0;
    std::string line;
    while (std::getline(input, line)) {
        std::stringstream ss(line);
        int l = 0;
        int r = 0;

        ss >> l >> r;

        l_list.push_back(l);
        r_list.push_back(r);

        lines++;
    }

    std::sort(l_list.begin(), l_list.end());
    std::sort(r_list.begin(), r_list.end());

    long long ans = 0;
    for (int i = 0; i < lines; i++) {
        ans += std::abs(l_list[i] - r_list[i]);
    }

    return ans;
}

long long part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    std::vector<int> l_list;
    std::unordered_map<int, int> r_freq;

    int lines = 0;
    std::string line;
    while (std::getline(input, line)) {
        std::stringstream ss(line);
        int l = 0;
        int r = 0;

        ss >> l >> r;

        l_list.push_back(l);
        r_freq[r]++;

        lines++;
    }

    long long ans = 0;
    for (int i = 0; i < lines; i++) {
        ans += static_cast<long long>(l_list[i]) * r_freq[l_list[i]];
    }

    return ans;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const long long part1_example_ans = 11;
    const long long part2_example_ans = 31;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}