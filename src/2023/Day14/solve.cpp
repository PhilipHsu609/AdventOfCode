#include <fmt/core.h> // fmt::print

#include <cassert>       // assert
#include <cstdlib>       // std::exit
#include <filesystem>    // std::filesystem
#include <fstream>       // std::ifstream
#include <string>        // std::getline
#include <unordered_map> // std::unordered_map

namespace fs = std::filesystem;

enum class Direction { UP, DOWN };

std::vector<std::string> read_input(std::ifstream &input) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    return lines;
}

void transpose(std::vector<std::string> &board) {
    std::vector<std::string> new_board(board[0].size(), std::string(board.size(), '.'));
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[0].size(); j++) {
            new_board[j][i] = board[i][j];
        }
    }
    board = new_board;
}

void tilt_boardh(std::vector<std::string> &board, Direction dir) {
    std::vector<size_t> write_indices(board[0].size(),
                                      dir == Direction::DOWN ? board.size() - 1 : 0);

    for (size_t i = 0; i < board.size(); i++) {
        size_t ii = dir == Direction::DOWN ? board.size() - i - 1 : i;
        for (size_t j = 0; j < board[0].size(); j++) {
            if (board[ii][j] == '#') {
                write_indices[j] = dir == Direction::DOWN ? ii - 1 : ii + 1;
            } else if (board[ii][j] == 'O') {
                if (board[write_indices[j]][j] != '.') {
                    write_indices[j] = dir == Direction::DOWN ? ii - 1 : ii + 1;
                } else {
                    board[ii][j] = '.';
                    board[write_indices[j]][j] = 'O';
                    write_indices[j] += dir == Direction::DOWN ? -1 : 1;
                }
            }
        }
    }
}

std::string board_to_string(const std::vector<std::string> &board) {
    std::string str;
    for (const auto &row : board) {
        str += row + '\n';
    }
    return str;
}

long long calculate_load(const std::vector<std::string> &board) {
    long long load = 0;
    int rows = static_cast<int>(board.size());
    for (const auto &row : board) {
        for (auto cell : row) {
            if (cell == 'O') {
                load += rows;
            }
        }
        rows--;
    }
    return load;
}

long long part1(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto board = read_input(input);
    tilt_boardh(board, Direction::UP);

    return calculate_load(board);
}

long long part2(const fs::path &input_path) {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        fmt::print(stderr, "File not found\n");
        std::exit(1);
    }

    auto board = read_input(input);
    const size_t iterations = 1000000000;

    std::unordered_map<std::string, size_t> seen;
    size_t cycle_start = 0;
    size_t cycle_length = 0;
    size_t remain = 0;

    for (size_t i = 0; i < iterations; i++) {
        tilt_boardh(board, Direction::UP);
        transpose(board);
        tilt_boardh(board, Direction::UP);
        transpose(board);
        tilt_boardh(board, Direction::DOWN);
        transpose(board);
        tilt_boardh(board, Direction::DOWN);
        transpose(board);

        std::string board_str = board_to_string(board);
        if (seen.find(board_str) != seen.end()) {
            cycle_start = seen[board_str];
            cycle_length = i - cycle_start;
            remain = (iterations - i - 1) % cycle_length;
            break;
        }

        seen.insert({board_str, i});
    }

    for (size_t i = 0; i < remain; i++) {
        tilt_boardh(board, Direction::UP);
        transpose(board);
        tilt_boardh(board, Direction::UP);
        transpose(board);
        tilt_boardh(board, Direction::DOWN);
        transpose(board);
        tilt_boardh(board, Direction::DOWN);
        transpose(board);
    }

    return calculate_load(board);
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));

    const fs::path example = "./example.txt";
    const fs::path input = "./input.txt";
    const long long part1_example_ans = 136;
    const long long part2_example_ans = 64;

    assert(part1(example) == part1_example_ans && "Part1 example failed");
    assert(part2(example) == part2_example_ans && "Part2 example failed");

    fmt::print("Part1: {}\n", part1(input));
    fmt::print("Part2: {}\n", part2(input));
    return 0;
}