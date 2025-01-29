#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

const std::string input_path = "./input.txt";

void part1() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    int sum = 0;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        int i = 0, j = line.length() - 1;
        while (!isdigit(line[i])) i++;
        while (!isdigit(line[j])) j--;
        sum += (line[i] - '0') * 10 + (line[j] - '0');
    }

    input.close();

    std::cout << "Part1: " << sum << std::endl;
}

void part2() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    const std::unordered_map<std::string, int> dict{
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9}};

    int sum = 0;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);

        int first = -1, last = 0;
        for (std::size_t i = 0; i < line.length(); i++) {
            if (isdigit(line[i])) {
                if (first == -1) {
                    first = line[i] - '0';
                }
                last = line[i] - '0';
            } else {
                for (auto &[key, value] : dict) {
                    if (line.substr(i, key.length()) == key) {
                        if (first == -1) {
                            first = value;
                        }
                        last = value;
                    }
                }
            }
        }
        sum += first * 10 + last;
    }

    input.close();

    std::cout << "Part2: " << sum << std::endl;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1();
    part2();
    return 0;
}