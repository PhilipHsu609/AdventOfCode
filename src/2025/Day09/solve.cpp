#include "utils.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Point {
    long long x;
    long long y;
};

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);

    std::vector<Point> points;
    points.reserve(lines.size());

    for (const auto &line : lines) {
        auto parts = utils::split(line, ",");
        points.push_back({std::stoll(parts[0]), std::stoll(parts[1])});
    }
    return points;
}

// Helper functions here

long long area(Point a, Point b) {
    return (std::abs(a.x - b.x) + 1) * (std::abs(a.y - b.y) + 1);
}

template <typename T>
auto part1(const T &input) {
    long long ret = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = i + 1; j < input.size(); ++j) {
            ret = std::max(ret, area(input[i], input[j]));
        }
    }
    return ret;
}

// Extract unique coordinates and sort them
std::vector<long long> get_unique_coords(const std::vector<Point> &points, bool is_x) {
    std::vector<long long> coords;
    coords.reserve(points.size());
    for (const auto &p : points) {
        coords.push_back(is_x ? p.x : p.y);
    }
    std::sort(coords.begin(), coords.end());
    coords.erase(std::unique(coords.begin(), coords.end()), coords.end());
    return coords;
}

// Map a real coordinate to its compressed index (2 * index)
// We use 2*index so that odd indices (2*i + 1) represent the GAPS between coordinates.
int get_compressed_idx(long long val, const std::vector<long long> &coords) {
    auto it = std::lower_bound(coords.begin(), coords.end(), val);
    auto idx = std::distance(coords.begin(), it);
    return static_cast<int>(idx) * 2;
}

template <typename T>
auto part2(const T &input) {
    // 1. Coordinate Compression
    auto unique_x = get_unique_coords(input, true);
    auto unique_y = get_unique_coords(input, false);

    // Compressed Grid Dimensions
    // Size = 2 * N + 1 (for gaps) + 2 (for padding/moat)
    int c_width = unique_x.size() * 2 + 3;
    int c_height = unique_y.size() * 2 + 3;

    // Grid: 0=Empty, 1=Border, 2=Exterior
    // Offset everything by +1 so (0,0) is always the "Moat" (Exterior)
    std::vector<std::vector<int8_t>> grid(c_height, std::vector<int8_t>(c_width, 0));

    // 2. Draw Borders on Compressed Grid
    size_t n = input.size();
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;

        // Get compressed indices (mapped to 2, 4, 6...)
        int x1 = get_compressed_idx(input[i].x, unique_x) + 1; // +1 for Moat
        int y1 = get_compressed_idx(input[i].y, unique_y) + 1;
        int x2 = get_compressed_idx(input[j].x, unique_x) + 1;
        int y2 = get_compressed_idx(input[j].y, unique_y) + 1;

        int cx_min = std::min(x1, x2);
        int cx_max = std::max(x1, x2);
        int cy_min = std::min(y1, y2);
        int cy_max = std::max(y1, y2);

        // Fill the line on the compressed grid
        if (x1 == x2) { // Vertical
            for (int y = cy_min; y <= cy_max; ++y) {
                grid[y][x1] = 1;
            }
        } else { // Horizontal
            for (int x = cx_min; x <= cx_max; ++x) {
                grid[y1][x] = 1;
            }
        }
    }

    // 3. Iterative Flood Fill (Exterior)
    // Start from (0,0) which is guaranteed to be the empty moat
    std::vector<std::pair<int, int>> stack;
    stack.emplace_back(0, 0);
    grid[0][0] = 2; // Mark Exterior

    const std::array<int, 4> dx{{0, 0, 1, -1}};
    const std::array<int, 4> dy{{1, -1, 0, 0}};

    while (!stack.empty()) {
        auto [cx, cy] = stack.back();
        stack.pop_back();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && nx < c_width && ny >= 0 && ny < c_height) {
                if (grid[ny][nx] == 0) { // If Empty
                    grid[ny][nx] = 2;    // Mark Exterior
                    stack.emplace_back(nx, ny);
                }
            }
        }
    }

    // 4. Build Validity Prefix Sum
    // We only care if a cell is VALID (Interior or Border).
    // Valid = (grid != 2).
    std::vector<std::vector<int>> psum(c_height + 1, std::vector<int>(c_width + 1, 0));

    for (int y = 0; y < c_height; ++y) {
        for (int x = 0; x < c_width; ++x) {
            int is_valid = (grid[y][x] != 2) ? 1 : 0;
            psum[y + 1][x + 1] = is_valid + psum[y][x + 1] + psum[y + 1][x] - psum[y][x];
        }
    }

    // 5. Iterate Pairs and Check Validity
    long long max_area = 0;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            // Get original coordinates for AREA calculation
            long long area_val = area(input[i], input[j]);

            if (area_val <= max_area) {
                continue;
            }

            // Get compressed indices for VALIDITY check
            int x1 = get_compressed_idx(input[i].x, unique_x) + 1;
            int y1 = get_compressed_idx(input[i].y, unique_y) + 1;
            int x2 = get_compressed_idx(input[j].x, unique_x) + 1;
            int y2 = get_compressed_idx(input[j].y, unique_y) + 1;

            int cx_min = std::min(x1, x2);
            int cx_max = std::max(x1, x2);
            int cy_min = std::min(y1, y2);
            int cy_max = std::max(y1, y2);

            // Calculate expected valid cells count in compressed grid
            // (width in compressed pixels) * (height in compressed pixels)
            int target_count = (cx_max - cx_min + 1) * (cy_max - cy_min + 1);

            // Query Prefix Sum
            int actual_count = psum[cy_max + 1][cx_max + 1] - psum[cy_min][cx_max + 1] -
                               psum[cy_max + 1][cx_min] + psum[cy_min][cx_min];

            if (actual_count == target_count) {
                max_area = area_val;
            }
        }
    }

    return max_area;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 50LL;
    const auto part2_example_ans = 24LL;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}