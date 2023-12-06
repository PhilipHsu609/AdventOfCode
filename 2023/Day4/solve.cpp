#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

std::vector<std::vector<int>> parseLine(std::ifstream& input) {
    std::string line;
    std::getline(input, line, ':');
    std::getline(input, line, '|');
    std::istringstream iss(line);

    std::vector<int> winNums;
    int num;
    while (iss >> num) {
        winNums.push_back(num);
    }

    std::getline(input, line);
    iss.str(line);
    iss.clear();

    std::vector<int> nums;
    while (iss >> num) {
        nums.push_back(num);
    }

    return {winNums, nums};
}

void part1() {
    std::ifstream input("input.txt");
    std::string line;

    int sumPoints = 0;
    while (!input.eof()) {
        int points = 0;
        auto tmp{parseLine(input)};
        auto winNums{tmp[0]};
        auto nums{tmp[1]};

        std::sort(winNums.begin(), winNums.end());
        for(int n : nums) {
            if (std::binary_search(winNums.begin(), winNums.end(), n)) {
                points = (points == 0) ? 1 : points * 2;
            }
        }

        sumPoints += points;
    }
    std::cout << "Part1: " << sumPoints << std::endl;
}

void part2() {
    std::ifstream input("input.txt");
    std::string line;

    std::vector<int> matches;
    while (!input.eof()) {
        int match = 0;
        auto tmp{parseLine(input)};
        auto winNums{tmp[0]};
        auto nums{tmp[1]};

        std::sort(winNums.begin(), winNums.end());
        for(int n : nums) {
            if (std::binary_search(winNums.begin(), winNums.end(), n)) {
                match++;
            }
        }
        matches.push_back(match);
    }

    // deal with the copies
    std::vector<int> copies(matches.size(), 1);
    for(int i = 0; i < matches.size(); i++) {
        for(int j = 1; j <= matches[i]; j++) {
            copies[i + j] += copies[i];
        }
    }

    std::cout << "Part2: " << std::accumulate(copies.begin(), copies.end(), 0) << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}