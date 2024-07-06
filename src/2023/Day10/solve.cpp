#include <fmt/core.h> // print

#include <array>         // array
#include <cassert>       // assert
#include <cmath>         // abs
#include <cstdlib>       // exit
#include <filesystem>    // path, current_path, absolute
#include <fstream>       // ifstream
#include <string>        // string, getline
#include <string_view>   // string_view
#include <unordered_set> // unordered_set
#include <vector>        // vector

namespace fs = std::filesystem;

using Point = std::array<int, 2>;

enum class Direction { UP, DOWN, LEFT, RIGHT };

Direction find_direction(const Point &cur, const std::vector<std::string> &grid) {
    static const std::unordered_set<char> left{'-', 'L', 'F'};
    static const std::unordered_set<char> right{'-', 'J', '7'};
    static const std::unordered_set<char> up{'|', '7', 'F'};
    static const std::unordered_set<char> down{'|', 'J', 'L'};

    auto [x, y] = cur;
    const int n = static_cast<int>(grid[0].size());

    if (y > 0 && left.find(grid[x][y - 1]) != left.end()) {
        return Direction::LEFT;
    }
    if (y < n - 1 && right.find(grid[x][y + 1]) != right.end()) {
        return Direction::RIGHT;
    }
    if (x > 0 && up.find(grid[x - 1][y]) != up.end()) {
        return Direction::UP;
    }

    return Direction::DOWN;
}

Point move_to(Point cur, Direction &dir, const std::vector<std::string> &grid) {
    switch (dir) {
    case Direction::UP:
        cur[0]--;
        break;
    case Direction::DOWN:
        cur[0]++;
        break;
    case Direction::LEFT:
        cur[1]--;
        break;
    case Direction::RIGHT:
        cur[1]++;
        break;
    }

    switch (grid[cur[0]][cur[1]]) {
    case 'L':
        dir = dir == Direction::DOWN ? Direction::RIGHT : Direction::UP;
        break;
    case 'J':
        dir = dir == Direction::DOWN ? Direction::LEFT : Direction::UP;
        break;
    case '7':
        dir = dir == Direction::UP ? Direction::LEFT : Direction::DOWN;
        break;
    case 'F':
        dir = dir == Direction::UP ? Direction::RIGHT : Direction::DOWN;
        break;
    case '|':
    case '-':
    default:
        break;
    }

    return cur;
}

Point parse_input(std::ifstream &is, std::vector<std::string> &grid) {
    std::string line;
    Point start{-1, -1};

    while (std::getline(is, line)) {
        const std::string_view sv(line);
        for (size_t i = 0; i < sv.size() && start[0] == -1; ++i) {
            if (sv[i] == 'S') {
                start[0] = static_cast<int>(grid.size());
                start[1] = static_cast<int>(i);
                break;
            }
        }
        grid.emplace_back(line);
    }
    return start;
}

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    std::vector<std::string> grid;
    const Point start = parse_input(input, grid);

    // Traversal
    Direction dir = find_direction(start, grid);
    Point cur = start;
    int steps = 0;

    do {
        cur = move_to(cur, dir, grid);
        steps++;
    } while (cur != start);

    return steps / 2;
}

int part2(const fs::path &input_path) {
    // Pick's theorem & Shoelace formula

    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    std::vector<std::string> grid;
    const Point start = parse_input(input, grid);

    // Traversal
    Direction dir = find_direction(start, grid);
    Point pre = start;
    Point cur;

    int area = 0;
    int steps = 0;

    do {
        cur = move_to(pre, dir, grid);

        // Shoelace formula
        area += pre[0] * cur[1] - pre[1] * cur[0];

        pre = cur;
        steps++;
    } while (cur != start);

    // Pick's theorem
    return (std::abs(area) - steps + 1) / 2 + 1;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example1 = "./example1.txt";
    const fs::path example2 = "./example2.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 8;
    const int part2_example_ans = 10;

    assert(part1(example1) == part1_example_ans && "Part1 example failed");
    assert(part2(example2) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}