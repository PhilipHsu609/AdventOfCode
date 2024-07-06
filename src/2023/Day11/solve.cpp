#include <fmt/core.h>   // print
#include <fmt/ranges.h> // print container

#include <array>      // array
#include <cassert>    // assert
#include <cmath>      // abs
#include <cstdlib>    // exit
#include <filesystem> // path, current_path, absolute
#include <fstream>    // ifstream
#include <string>     // string, getline
#include <vector>     // vector

namespace fs = std::filesystem;

using Point = std::array<int, 2>;

std::vector<std::string> read_input(std::ifstream &is) {
    std::string line;
    std::vector<std::string> ret;
    while (std::getline(is, line)) {
        ret.emplace_back(line);
    }
    return ret;
}

long long part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto grid = read_input(input);

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    std::vector<int> empty_rows(rows + 1, 1);
    std::vector<int> empty_cols(cols + 1, 1);
    std::vector<Point> galaxies;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '#') {
                galaxies.push_back({i, j});
                empty_rows[i + 1] = 0;
                empty_cols[j + 1] = 0;
            }
        }
    }

    // Calculate prefix sum
    empty_cols[0] = empty_rows[0] = 0;
    for (int i = 1; i < rows + 1; i++) {
        empty_rows[i] += empty_rows[i - 1];
    }
    for (int i = 1; i < cols + 1; i++) {
        empty_cols[i] += empty_cols[i - 1];
    }

    const int points = static_cast<int>(galaxies.size());
    long long distance = 0;

    for (int i = 0; i < points; i++) {
        for (int j = i + 1; j < points; j++) {
            const int x1 = galaxies[i][0];
            const int y1 = galaxies[i][1];
            const int x2 = galaxies[j][0];
            const int y2 = galaxies[j][1];

            const int dx = std::abs(x1 - x2);
            const int dy = std::abs(y1 - y2);

            // Manhattan distance
            distance += dx + dy;
            // The number of empty rows/cols between two galaxies
            distance += empty_rows[std::max(x1, x2) + 1] - empty_rows[std::min(x1, x2)];
            distance += empty_cols[std::max(y1, y2) + 1] - empty_cols[std::min(y1, y2)];
        }
    }

    return distance;
}

long long part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto grid = read_input(input);

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    std::vector<int> empty_rows(rows + 1, 1);
    std::vector<int> empty_cols(cols + 1, 1);
    std::vector<Point> galaxies;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '#') {
                galaxies.push_back({i, j});
                empty_rows[i + 1] = 0;
                empty_cols[j + 1] = 0;
            }
        }
    }

    // Calculate prefix sum
    empty_cols[0] = empty_rows[0] = 0;
    for (int i = 1; i < rows + 1; i++) {
        empty_rows[i] += empty_rows[i - 1];
    }
    for (int i = 1; i < cols + 1; i++) {
        empty_cols[i] += empty_cols[i - 1];
    }

    const int points = static_cast<int>(galaxies.size());
    const long long expand_factor = 1'000'000LL;
    long long distance = 0;

    for (int i = 0; i < points; i++) {
        for (int j = i + 1; j < points; j++) {
            const int x1 = galaxies[i][0];
            const int y1 = galaxies[i][1];
            const int x2 = galaxies[j][0];
            const int y2 = galaxies[j][1];

            const int dx = std::abs(x1 - x2);
            const int dy = std::abs(y1 - y2);

            // Manhattan distance
            distance += dx + dy;
            // The number of empty rows/cols between two galaxies
            const int empty_rows_cols =
                empty_rows[std::max(x1, x2) + 1] - empty_rows[std::min(x1, x2)] +
                empty_cols[std::max(y1, y2) + 1] - empty_cols[std::min(y1, y2)];
            // Each empty row/col is expanded by expand_factor, which means
            // expand_factor - 1 newly added empty rows/cols.
            distance += empty_rows_cols * (expand_factor - 1LL);
        }
    }

    return distance;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const long long part1_example_ans = 374LL;
    const long long part2_example_ans = 82000210LL;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}