#include <fmt/core.h> // fmt::print

#include <array>         // std::array
#include <cassert>       // assert
#include <cstdlib>       // std::exit
#include <filesystem>    // std::filesystem
#include <fstream>       // std::ifstream
#include <queue>         // std::priority_queue
#include <string>        // std::getline
#include <unordered_set> // std::unordered_set
#include <utility>       // std::move
#include <vector>        // std::vector

namespace fs = std::filesystem;

std::vector<std::vector<int>> read_input(std::ifstream &input) {
    std::vector<std::vector<int>> data;
    std::string line;

    while (std::getline(input, line)) {
        std::vector<int> row;
        row.reserve(line.size());
        for (char c : line) {
            row.push_back(c - '0');
        }
        data.push_back(std::move(row));
    }

    return data;
}

namespace std {
template <>
struct hash<std::array<int, 5>> {
    size_t operator()(const std::array<int, 5> &arr) const {
        size_t seed = 0;
        for (int val : arr) {
            seed ^= std::hash<int>{}(val) + 0x9e3779b9 + (seed << 16) + (seed >> 16);
        }
        return seed;
    }
};
} // namespace std

int calculate_heat(const std::vector<std::vector<int>> &grid, int minSteps = 1,
                   int maxSteps = 3) {
    int m = static_cast<int>(grid.size());
    int n = static_cast<int>(grid[0].size());

    std::vector<int> dir{0, 1, 0, -1, 0};

    // loss, y, x, dy, dx, consecutive direction count
    std::priority_queue<std::array<int, 6>, std::vector<std::array<int, 6>>,
                        std::greater<>>
        q;

    q.push({0, 0, 0, 0, 1, 0});
    q.push({0, 0, 0, 1, 0, 0});

    // y, x, dy, dx, consecutive direction count
    std::unordered_set<std::array<int, 5>> visited;

    visited.insert({0, 0, 1, 0, 0});
    visited.insert({0, 0, 0, 1, 0});

    while (!q.empty()) {
        auto [l, y, x, dy, dx, consecutive] = q.top();
        q.pop();

        if (y == m - 1 && x == n - 1) {
            if (consecutive < minSteps) {
                continue;
            }
            return l;
        }

        for (int d = 0; d < 4; d++) {
            int ny = y + dir[d];
            int nx = x + dir[d + 1];

            int ndy = dir[d];
            int ndx = dir[d + 1];

            if (ny < 0 || ny >= m || nx < 0 || nx >= n) {
                continue;
            }
            if (dy == -ndy && dx == -ndx) {
                continue;
            }
            if (consecutive >= maxSteps && dy == ndy && dx == ndx) {
                continue;
            }
            if (consecutive < minSteps && (dy != ndy || dx != ndx)) {
                continue;
            }

            int nconsecutive = (dy == ndy && dx == ndx) ? consecutive + 1 : 1;
            int nloss = l + grid[ny][nx];

            if (visited.find({ny, nx, ndy, ndx, nconsecutive}) != visited.end()) {
                continue;
            }

            visited.insert({ny, nx, ndy, ndx, nconsecutive});
            q.push({nloss, ny, nx, ndy, ndx, nconsecutive});
        }
    }

    return -1;
}

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    std::vector<std::vector<int>> grid = read_input(input);

    return calculate_heat(grid);
}

int part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }
    std::vector<std::vector<int>> grid = read_input(input);

    return calculate_heat(grid, 4, 10);
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 102;
    const int part2_example_ans = 94;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}