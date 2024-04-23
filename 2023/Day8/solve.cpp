#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

const std::string input_path = "./input.txt";

enum {
    LEFT,
    RIGHT,
};

void part1() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    std::regex pattern("(\\w+) = \\((\\w+), (\\w+)\\)");
    std::smatch matches;
    std::string line, inst;

    std::unordered_map<std::string, std::vector<std::string>> graph;

    // Get the instruction
    std::getline(input, inst);

    while (std::getline(input, line)) {
        if (std::regex_match(line, matches, pattern)) {
            graph[matches[1]].push_back(matches[2]);
            graph[matches[1]].push_back(matches[3]);
        }
    }
    input.close();

    int steps = 0;
    std::string node("AAA");
    while (node != "ZZZ") {
        for (char c : inst) {
            steps++;

            if (c == 'L') {
                node = graph[node][LEFT];
            } else {
                node = graph[node][RIGHT];
            }

            if (node == "ZZZ")
                break;
        }
    }

    std::cout << "Problem 1: " << steps << std::endl;
}

long long lcm(long long a, long long b) {
    return a * b / std::__gcd(a, b);
}

void part2() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    std::regex pattern("(\\w+) = \\((\\w+), (\\w+)\\)");
    std::smatch matches;
    std::string line, inst;

    std::unordered_map<std::string, std::vector<std::string>> graph;

    // Get the instruction
    std::getline(input, inst);

    std::vector<std::string> nodes;
    while (std::getline(input, line)) {
        if (std::regex_match(line, matches, pattern)) {
            if (matches[1].str().back() == 'A')
                nodes.push_back(matches[1]);

            graph[matches[1]].push_back(matches[2]);
            graph[matches[1]].push_back(matches[3]);
        }
    }
    input.close();

    /*
        In the second problem, each starting node will enter a cycle after following the instructions,
        and there is exactly one destination node (i.e., node ending with 'Z') along the path.
        We also found that the destination node is at the end of the cycle.
        If we took u steps to reach the destination node, then we will reach the destination node again after u steps.
    */
    std::vector<int> steps;
    for (auto &node : nodes) {
        int step = 0;
        bool stop = false;
        std::unordered_map<std::string, int> seen;
        std::string u = node;

        while (!stop) {
            for (char c : inst) {
                step++;
                if (c == 'L') {
                    u = graph[u][LEFT];
                } else {
                    u = graph[u][RIGHT];
                }

                if (u.back() == 'Z') {
                    if (seen.find(u) != seen.end()) {
                        std::cout << "Cycle detected at " << u << " after " << step - seen[u] << " steps begin at step " << seen[u] << std::endl;
                        stop = true;
                    } else {
                        seen[u] = step;
                        steps.push_back(step);
                    }
                }
            }
        }
    }

    // Calculate the LCM of all steps
    long long lcm = steps[0];
    int n = steps.size();
    for (int i = 1; i < n; i++) {
        lcm = ::lcm(lcm, steps[i]);
    }

    std::cout << "Problem 2: " << lcm << std::endl;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1();
    part2();
    return 0;
}