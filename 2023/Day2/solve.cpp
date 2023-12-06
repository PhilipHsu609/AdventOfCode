#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

struct Sample {
    int red{};
    int green{};
    int blue{};
};

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<Sample> parseLine(std::vector<std::string> &vec) {
    std::vector<Sample> samples;
    std::smatch matches;

    for (auto &sample : vec) {
        Sample s;
        std::regex pattern{"(\\d+) (red|green|blue)"};
        while (std::regex_search(sample, matches, pattern)) {
            if (matches[2] == "red") {
                s.red = std::stoi(matches[1]);
            } else if (matches[2] == "green") {
                s.green = std::stoi(matches[1]);
            } else if (matches[2] == "blue") {
                s.blue = std::stoi(matches[1]);
            }
            sample = matches.suffix();
        }
        samples.push_back(s);
    }

    return samples;
}

bool isPossible(const Sample &s) {
    return s.red <= 12 && s.green <= 13 && s.blue <= 14;
}

void part1() {
    std::ifstream input{"input.txt"};
    std::string line;

    int sum = 0;
    std::smatch matches;

    while (!input.eof()) {
        std::getline(input, line);
        std::regex pattern{"^Game (\\d+):"};
        bool flag{true};

        int id{-1};

        std::regex_search(line, matches, pattern);
        id = std::stoi(matches[1]);

        auto samples{split(line.substr(line.find(':') + 1), ';')};
        auto parsed_samples{parseLine(samples)};

        for (auto &sample : parsed_samples) {
            if (!isPossible(sample)) {
                flag = false;
                break;
            }
        }

        if (flag) {
            sum += id;
        }
    }

    std::cout << "Part1: " << sum << std::endl;
}

void part2() {
    std::ifstream input{"input.txt"};
    std::string line;

    int sum{};
    std::smatch matches;

    while (!input.eof()) {
        std::getline(input, line);
        std::regex pattern{"^Game (\\d+):"};
        bool flag{true};

        auto samples{split(line.substr(line.find(':') + 1), ';')};
        auto parsed_samples{parseLine(samples)};

        Sample fewest;
        for (auto &sample : parsed_samples) {
            fewest.red = std::max(fewest.red, sample.red);
            fewest.green = std::max(fewest.green, sample.green);
            fewest.blue = std::max(fewest.blue, sample.blue);
        }

        sum += fewest.red * fewest.green * fewest.blue;
    }

    std::cout << "Part2: " << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}