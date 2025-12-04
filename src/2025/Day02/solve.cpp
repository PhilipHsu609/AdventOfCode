#include "utils.hpp"

#include <fmt/core.h>

#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Range {
    long long start;
    long long end;
};

// Custom input parser - modify based on problem
auto read_input(const fs::path &path) {
    auto lines = utils::read_lines(path);
    std::vector<Range> ranges;
    for (const auto &part : utils::split(lines[0], ',')) {
        Range r{};
        auto it = part.find('-');
        r.start = std::stoll(part.substr(0, it));
        r.end = std::stoll(part.substr(it + 1));
        ranges.push_back(r);
    }
    return ranges;
}

// Helper functions here

std::vector<int> get_divisors(int num) {
    std::vector<int> divisors;
    for (int i = 1; i < num; i++) {
        if (num % i == 0) {
            divisors.push_back(i);
        }
    }
    return divisors;
}

template <typename T>
auto part1(const T &input) {
    auto is_valid = [](long long num) {
        // Count digits
        long long tmp = num;
        int digits = 0;

        while (tmp > 0) {
            tmp /= 10;
            digits++;
        }

        if (digits % 2 == 1) {
            return false;
        }

        // Split into two halves
        long long half_divisor = 1;
        for (int i = 0; i < digits / 2; ++i) {
            half_divisor *= 10;
        }
        long long first_half = num / half_divisor;
        long long second_half = num % half_divisor;

        return first_half == second_half;
    };
    long long ret = 0;
    for (const auto &range : input) {
        for (long long i = range.start; i <= range.end; ++i) {
            if (is_valid(i)) {
                ret += i;
            }
        }
    }
    return ret;
}

template <typename T>
auto part2(const T &input) {
    auto is_valid = [](long long num) {
        // Count digits
        long long tmp = num;
        int digits = 0;

        while (tmp > 0) {
            tmp /= 10;
            digits++;
        }

        auto divisors = get_divisors(digits);
        for (auto divisor : divisors) {
            long long part_divisor = 1;
            for (int i = 0; i < divisor; ++i) {
                part_divisor *= 10;
            }
            // Split num into parts of size divisor
            bool all_equal = true;
            long long first_part = num % part_divisor;
            long long check_num = num / part_divisor;
            for (int i = 1; i < digits / divisor; ++i) {
                long long current_part = check_num % part_divisor;
                if (current_part != first_part) {
                    all_equal = false;
                    break;
                }
                check_num /= part_divisor;
            }
            if (all_equal) {
                return true;
            }
        }
        return false;
    };

    long long ret = 0;
    for (const auto &range : input) {
        for (long long i = range.start; i <= range.end; ++i) {
            if (is_valid(i)) {
                ret += i;
            }
        }
    }
    return ret;
}

int main() {
    utils::set_working_dir(__FILE__);

    const auto example = read_input("./example.txt");
    const auto input = read_input("./input.txt");
    const auto part1_example_ans = 1227775554LL;
    const auto part2_example_ans = 4174379265LL;

    utils::assert_eq(part1(example), part1_example_ans, "Part1 example failed");
    utils::assert_eq(part2(example), part2_example_ans, "Part2 example failed");

    utils::Timer timer;
    auto p1 = part1(input);
    fmt::print("Part1: {} ({:.3f}ms)\n", p1, timer.elapsed_and_reset());

    auto p2 = part2(input);
    fmt::print("Part2: {} ({:.3f}ms)\n", p2, timer.elapsed());

    return 0;
}