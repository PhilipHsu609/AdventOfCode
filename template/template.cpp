#include <fmt/core.h> // print

#include <cassert>    // assert
#include <cstdlib>    // exit
#include <filesystem> // path, current_path, absolute
#include <fstream>    // ifstream
#include <string>     // string, getline

namespace fs = std::filesystem;

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    return 0;
}

int part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    return 0;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 0;
    const int part2_example_ans = 0;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}", part1(input));
    fmt::print("Part2: {}", part2(input));
    return 0;
}