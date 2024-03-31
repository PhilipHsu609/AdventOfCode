#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

const std::string input_path = "./input.txt";

void part1() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }
}

void part2() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1();
    part2();
    return 0;
}