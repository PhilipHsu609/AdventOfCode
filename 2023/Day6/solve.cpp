#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace fs = std::filesystem;

const std::string input_path = "./input.txt";

void part1() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    std::string line;
    std::smatch matches;
    std::regex pattern("\\d+");

    std::vector<int> time, dist;

    std::getline(input, line);
    while (std::regex_search(line, matches, pattern)) {
        time.push_back(std::stoi(matches[0]));
        line = matches.suffix();
    }

    std::getline(input, line);
    while (std::regex_search(line, matches, pattern)) {
        dist.push_back(std::stoi(matches[0]));
        line = matches.suffix();
    }

    int ans = 1;
    for (std::size_t i = 0; i < time.size(); i++) {
        int low = std::ceil((time[i] - std::sqrt(time[i] * time[i] - 4 * dist[i])) / 2.0);
        int high = std::floor((time[i] + std::sqrt(time[i] * time[i] - 4 * dist[i])) / 2.0);
        ans *= high - low + 1;
    }

    std::cout << "Part1: " << ans << std::endl;
}

void part2() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    std::string line;
    std::smatch matches;
    std::regex pattern("\\d+");

    std::string time_s, dist_s;

    std::getline(input, line);
    while (std::regex_search(line, matches, pattern)) {
        time_s += matches[0];
        line = matches.suffix();
    }

    std::getline(input, line);
    while (std::regex_search(line, matches, pattern)) {
        dist_s += matches[0];
        line = matches.suffix();
    }

    long long time = std::stoll(time_s);
    long long dist = std::stoll(dist_s);

    long long low = std::ceil((time - std::sqrt(time * time - 4 * dist)) / 2.0);
    long long high = std::floor((time + std::sqrt(time * time - 4 * dist)) / 2.0);

    std::cout << "Part2: " << high - low + 1 << std::endl;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1();
    part2();
    return 0;
}