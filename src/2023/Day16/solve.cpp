#include <fmt/core.h> // fmt::print

#include <array>      // std::array
#include <cassert>    // assert
#include <cstdlib>    // std::exit
#include <filesystem> // std::filesystem
#include <fstream>    // std::ifstream
#include <queue>      // std::queue
#include <string>     // std::getline

namespace fs = std::filesystem;

std::vector<std::string> read_input(std::ifstream &input) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    return lines;
}

constexpr int NORTH = 1;
constexpr int EAST = 1 << 1;
constexpr int SOUTH = 1 << 2;
constexpr int WEST = 1 << 3;

std::vector<std::vector<int>> emit_beam(const std::vector<std::string> &grid, int x,
                                        int y, int dir) {
    int m = static_cast<int>(grid.size());
    int n = static_cast<int>(grid[0].size());

    std::vector<std::vector<int>> energy(m, std::vector<int>(n));
    std::queue<std::array<int, 3>> q;

    q.push({x, y, dir});

    while (!q.empty()) {
        auto [i, j, d] = q.front();
        q.pop();

        if (i < 0 || j < 0 || i >= m || j >= n) {
            continue;
        }

        if ((energy[i][j] & d) != 0) {
            continue;
        }

        energy[i][j] |= d;

        switch (grid[i][j]) {
        case '/':
            if (d == NORTH) {
                q.push({i, j + 1, EAST});
            } else if (d == EAST) {
                q.push({i - 1, j, NORTH});
            } else if (d == SOUTH) {
                q.push({i, j - 1, WEST});
            } else if (d == WEST) {
                q.push({i + 1, j, SOUTH});
            }
            break;
        case '\\':
            if (d == NORTH) {
                q.push({i, j - 1, WEST});
            } else if (d == EAST) {
                q.push({i + 1, j, SOUTH});
            } else if (d == SOUTH) {
                q.push({i, j + 1, EAST});
            } else if (d == WEST) {
                q.push({i - 1, j, NORTH});
            }
            break;
        case '-':
            if (d == NORTH || d == SOUTH) {
                q.push({i, j + 1, EAST});
                q.push({i, j - 1, WEST});
            } else if (d == EAST) {
                q.push({i, j + 1, EAST});
            } else if (d == WEST) {
                q.push({i, j - 1, WEST});
            }
            break;
        case '|':
            if (d == EAST || d == WEST) {
                q.push({i + 1, j, SOUTH});
                q.push({i - 1, j, NORTH});
            } else if (d == NORTH) {
                q.push({i - 1, j, NORTH});
            } else if (d == SOUTH) {
                q.push({i + 1, j, SOUTH});
            }
            break;
        case '.':
            if (d == NORTH) {
                q.push({i - 1, j, NORTH});
            } else if (d == EAST) {
                q.push({i, j + 1, EAST});
            } else if (d == SOUTH) {
                q.push({i + 1, j, SOUTH});
            } else if (d == WEST) {
                q.push({i, j - 1, WEST});
            }
            break;
        default:
            break;
        }
    }

    return energy;
}

int calculate_energy(std::vector<std::vector<int>> &energy) {
    int cnt = 0;
    for (const auto &row : energy) {
        for (const auto &cell : row) {
            if (cell != 0) {
                cnt++;
            }
        }
    }
    return cnt;
}

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto grid = read_input(input);
    auto energy = emit_beam(grid, 0, 0, EAST);

    return calculate_energy(energy);
}

int part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto grid = read_input(input);
    std::vector<std::vector<int>> energy;

    int m = static_cast<int>(grid.size());
    int n = static_cast<int>(grid[0].size());

    int ret = 0;
    for (int i = 0; i < m; i++) {
        energy = emit_beam(grid, i, 0, EAST);
        ret = std::max(ret, calculate_energy(energy));

        energy = emit_beam(grid, i, n - 1, WEST);
        ret = std::max(ret, calculate_energy(energy));
    }

    for (int j = 0; j < n; j++) {
        energy = emit_beam(grid, 0, j, SOUTH);
        ret = std::max(ret, calculate_energy(energy));

        energy = emit_beam(grid, m - 1, j, NORTH);
        ret = std::max(ret, calculate_energy(energy));
    }

    return ret;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 46;
    const int part2_example_ans = 51;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}