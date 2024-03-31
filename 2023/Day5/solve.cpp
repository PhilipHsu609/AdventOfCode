#include <algorithm>
#include <climits>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../../Utils/utils.hpp"

namespace fs = std::filesystem;

using Range = std::tuple<long long, long long, long long>;

const std::string input_path = "./input.txt";

const std::vector<std::string> keys{
    "seed-to-soil map:",
    "soil-to-fertilizer map:",
    "fertilizer-to-water map:",
    "water-to-light map:",
    "light-to-temperature map:",
    "temperature-to-humidity map:",
    "humidity-to-location map:"};

void part1() {
    std::ifstream input(input_path);
    std::string line;
    std::smatch matches;

    if (!input.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::unordered_map<std::string, std::set<Range>> ranges;

    std::vector<long long> seeds;
    std::getline(input, line);
    std::regex pattern("\\d+");
    while (std::regex_search(line, matches, pattern)) {
        seeds.push_back(std::stoll(matches[0]));
        line = matches.suffix();
    }

    std::string cur_key;
    while (!input.eof()) {
        std::getline(input, line);

        if (line.empty()) {
            continue;
        }

        if (std::find(keys.begin(), keys.end(), line) != keys.end()) {
            cur_key = line;
        } else {
            std::regex pattern("(\\d+) (\\d+) (\\d+)");
            std::regex_search(line, matches, pattern);
            ranges[cur_key].insert(
                std::make_tuple(std::stoll(matches[2]), std::stoll(matches[1]), std::stoll(matches[3]))
                // (src, dst, len)
            );
        }
    }

    input.close();

    long long min_loc = LLONG_MAX;
    for (long long seed : seeds) {
        long long loc = seed;

        for (auto &key : keys) {
            for (auto &range : ranges[key]) {
                if (loc >= std::get<0>(range) && loc <= std::get<0>(range) + std::get<2>(range) - 1) {
                    loc = std::get<1>(range) + loc - std::get<0>(range);
                    break;
                }
            }
        }
        min_loc = std::min(min_loc, loc);
    }

    std::cout << "Part1: " << min_loc << std::endl;
}

void part2() {
    std::ifstream input(input_path);
    std::string line;
    std::smatch matches;

    if (!input.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::unordered_map<std::string, std::set<Range>> ranges;
    std::list<Range> seed_ranges;

    // Read seeds
    std::getline(input, line);
    std::vector<std::string> seeds = split(line, ' ');
    for (std::size_t i = 1; i < seeds.size(); i += 2) {
        long long seed_start = std::stoll(seeds[i]);
        long long len = std::stoll(seeds[i + 1]);

        seed_ranges.push_back(
            std::make_tuple(seed_start, seed_start + len, 0)
            // (seed_start, seed_end, level)
        );
    }

    // Read maps
    std::string cur_key;
    while (!input.eof()) {
        std::getline(input, line);

        if (line.empty()) {
            continue;
        }

        if (std::find(keys.begin(), keys.end(), line) != keys.end()) {
            cur_key = line;
        } else {
            std::regex pattern("(\\d+) (\\d+) (\\d+)");
            std::regex_search(line, matches, pattern);
            // (src, dst, len)
            ranges[cur_key].insert(
                std::make_tuple(std::stoll(matches[2]), std::stoll(matches[1]), std::stoll(matches[3])));
        }
    }

    input.close();

    // Find the minimum location
    long long min_loc = LLONG_MAX;
    while (!seed_ranges.empty()) {
        auto range = seed_ranges.front();
        seed_ranges.pop_front();

        // Current seed range
        long long src_start = std::get<0>(range);
        long long src_end = std::get<1>(range);
        long long level = std::get<2>(range);

        if (level == static_cast<long long>(keys.size())) {
            min_loc = std::min(min_loc, src_start);
            continue;
        }

        // Check if the current seed range intersects with any of the ranges
        bool overlap = false;
        for (auto &r : ranges[keys[level]]) {
            long long r_start = std::get<0>(r);
            long long r_end = std::get<0>(r) + std::get<2>(r);

            long long diff = std::get<1>(r) - std::get<0>(r);

            if (src_end <= r_start || src_start >= r_end) {
                continue;
            }

            if (src_start < r_start) {
                seed_ranges.push_back(
                    std::make_tuple(src_start, r_start, level));
                src_start = r_start;
            }

            if (src_end > r_end) {
                seed_ranges.push_back(
                    std::make_tuple(r_end, src_end, level));
                src_end = r_end;
            }

            seed_ranges.push_back(
                std::make_tuple(src_start + diff, src_end + diff, level + 1));
            overlap = true;
            break;
        }

        if (!overlap) {
            seed_ranges.push_back(
                std::make_tuple(src_start, src_end, level + 1));
        }
    }

    std::cout << "Part2: " << min_loc << std::endl;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1();
    part2();
    return 0;
}