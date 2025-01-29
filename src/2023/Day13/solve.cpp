#include <fmt/core.h> // fmt::print

#include <cassert>     // assert
#include <cstdlib>     // std::exit
#include <filesystem>  // std::filesystem
#include <fstream>     // std::ifstream
#include <string>      // std::getline
#include <string_view> // std::string_view
#include <utility>     // std::move
#include <vector>      // std::vector

namespace fs = std::filesystem;

std::vector<std::string> read_input(std::ifstream &input) {
    std::vector<std::string> lines;
    for (std::string line; std::getline(input, line) && !line.empty();) {
        lines.push_back(std::move(line));
    }
    return lines;
}

std::vector<std::string> transpose(const std::vector<std::string> &board) {
    std::vector<std::string> transposed(board[0].size(), std::string(board.size(), ' '));
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[0].size(); j++) {
            transposed[j][i] = board[i][j];
        }
    }
    return transposed;
}

size_t diff(std::string_view a, std::string_view b) {
    size_t ret = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            ret++;
        }
    }
    return ret;
}

int check_mirror(const std::vector<std::string> &board, size_t smudge_threshold = 0) {
    size_t n = board.size();

    for (size_t i = 1; i < n; i++) {
        if (diff(board[i - 1], board[i]) > smudge_threshold) {
            continue;
        }

        int l = static_cast<int>(i - 1);
        int r = static_cast<int>(i);
        int ret = static_cast<int>(i);

        size_t smudge = 0;
        while (l >= 0 && static_cast<size_t>(r) < n) {
            smudge += diff(board[l], board[r]);
            if (smudge > smudge_threshold) {
                ret = 0;
                break;
            }
            l--, r++;
        }

        if (ret != 0 && smudge == smudge_threshold) {
            return ret;
        }
    }

    return 0;
}

int part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    int ans = 0;
    while (!input.eof()) {
        auto board = read_input(input);

        // check horizontally
        int hans = check_mirror(board, 0);
        // check vertically
        int vans = check_mirror(transpose(board), 0);

        ans += vans + 100 * hans;
    }

    return ans;
}

int part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    int ans = 0;
    while (!input.eof()) {
        auto board = read_input(input);

        // check horizontally
        int hans = check_mirror(board, 1);
        // check vertically
        int vans = check_mirror(transpose(board), 1);

        ans += vans + 100 * hans;
    }

    return ans;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const int part1_example_ans = 405;
    const int part2_example_ans = 400;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}