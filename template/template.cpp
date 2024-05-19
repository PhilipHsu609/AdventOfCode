#include <fmt/core.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

void part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        return;
    }
}

void part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        return;
    }
}

int main() {
    const fs::path input_path = "./input.txt";
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1(input_path);
    part2(input_path);
    return 0;
}