#include <fmt/core.h>

#include <array>
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace fs = std::filesystem;

struct Dig {
    int dir;
    int steps;
    int color;
};

template <typename T>
std::array<std::array<T, 2>, 4> dirs = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

template <typename T, typename = std::enable_if<std::is_integral_v<T>>>
std::array<T, 2> operator*(const std::array<T, 2> &a, T b) {
    return {a[0] * b, a[1] * b};
}

template <typename T, typename = std::enable_if<std::is_integral_v<T>>>
std::array<T, 2> operator+(const std::array<T, 2> &a, const std::array<T, 2> &b) {
    return {a[0] + b[0], a[1] + b[1]};
}

std::vector<Dig> read_input(std::ifstream &input) {
    std::vector<Dig> digs;
    std::string line;
    while (std::getline(input, line)) {
        std::stringstream ss(line);
        char d{};
        int s{};
        std::string c;
        ss >> d >> s >> c;

        // Convert color to int (#RRGGBB) -> 0xRRGGBB
        int color = std::stoi(c.substr(2, 6), nullptr, 16);

        // Convert direction to index
        switch (d) {
        case 'U':
            d = 3;
            break;
        case 'R':
            d = 0;
            break;
        case 'D':
            d = 1;
            break;
        case 'L':
            d = 2;
            break;
        default:
            fmt::print(stderr, "Invalid direction: {}\n", d);
            std::exit(1);
        }

        digs.emplace_back(Dig{d, s, color});
    }
    return digs;
}

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto digs = read_input(input);

    std::array<int, 2> prev = {0, 0};
    std::array<int, 2> curr = {0, 0};

    int area = 0;
    int steps = 0;
    for (const auto &dig : digs) {
        // Calculate the shoelace formula
        area += prev[0] * curr[1] - prev[1] * curr[0];
        steps += dig.steps;

        prev = curr;
        curr = curr + dirs<int>[dig.dir] * dig.steps;
    }

    area /= 2;

    // Pick's theorem
    return area + 1 - steps / 2 + steps;
}

long long part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto digs = read_input(input);

    std::array<long long, 2> prev = {0, 0};
    std::array<long long, 2> curr = {0, 0};

    long long area = 0;
    long long steps = 0;

    for (auto &dig : digs) {
        dig.dir = dig.color & 0xF;
        dig.steps = dig.color >> 4;

        // Calculate the shoelace formula
        area += prev[0] * curr[1] - prev[1] * curr[0];
        steps += dig.steps;

        prev = curr;
        curr = curr + dirs<long long>[dig.dir] * static_cast<long long>(dig.steps);
    }

    area /= 2;

    // Pick's theorem
    return area + 1 - steps / 2 + steps;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 62;
    const long long part2_example_ans = 952408144115;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}