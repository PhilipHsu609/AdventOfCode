#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

const std::string input_path = "./input.txt";

const std::unordered_map<char, char> cards_map_1{
    {'2', '0'},
    {'3', '1'},
    {'4', '2'},
    {'5', '3'},
    {'6', '4'},
    {'7', '5'},
    {'8', '6'},
    {'9', '7'},
    {'T', '8'},
    {'J', '9'},
    {'Q', 'A'},
    {'K', 'B'},
    {'A', 'C'},
};

const std::unordered_map<char, char> cards_map_2{
    {'J', '0'},
    {'2', '1'},
    {'3', '2'},
    {'4', '3'},
    {'5', '4'},
    {'6', '5'},
    {'7', '6'},
    {'8', '7'},
    {'9', '8'},
    {'T', '9'},
    {'Q', 'A'},
    {'K', 'B'},
    {'A', 'C'},
};

std::vector<std::pair<std::string, int>> parse_input(std::ifstream &input, const std::unordered_map<char, char> &m) {
    std::vector<std::pair<std::string, int>> result;
    std::string line;

    while (!input.eof()) {
        std::getline(input, line);
        std::istringstream iss(line);
        std::string card;
        int value;

        iss >> card >> value;
        std::transform(card.begin(), card.end(), card.begin(), [&m](char c) {
            return m.at(c);
        });

        result.push_back(std::make_pair(card, value));
    }

    return result;
}

void part1() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    auto cards = parse_input(input, cards_map_1);
    auto cmp = [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
        // Count the number of cards for each hand.
        std::unordered_map<char, int> a_map, b_map;

        for (auto &c : a.first)
            a_map[c]++;

        for (auto &c : b.first)
            b_map[c]++;

        // The number of different cards
        int n_a = a_map.size(), n_b = b_map.size();

        // If the number of different cards is different, the one with less different cards wins.
        //     e.g. Five of a kind vs. Two pairs.
        if (n_a != n_b)
            return n_a > n_b;

        // If the number of different cards is same, a and b may have the same strength,
        // so we need to compare the card from the first one to the last one.
        // Except for two cases:
        //     1. Four of a kind and Full house.
        //     2. Three of a kind and Two pairs.
        if (n_a != 2 && n_a != 3)
            return a.first < b.first;

        // Deal with the two cases.
        // They can be handled by finding the maximun number of the cards.
        int a_max = 0, b_max = 0;
        for (auto &p : a_map)
            a_max = std::max(a_max, p.second);
        for (auto &p : b_map)
            b_max = std::max(b_max, p.second);

        if (a_max != b_max)
            return a_max < b_max;
        else
            return a.first < b.first;
    };

    std::sort(cards.begin(), cards.end(), cmp);

    long long winnings = 0;
    int rank = 1;
    for (auto &card : cards) {
        winnings += (rank++) * card.second;
    }

    std::cout << "Part 1: " << winnings << std::endl;
}

void part2() {
    std::ifstream input(input_path);

    if (!input.is_open()) {
        std::cerr << "File not found" << std::endl;
        return;
    }

    const char joker = cards_map_2.at('J');
    auto cards = parse_input(input, cards_map_2);
    auto cmp = [&joker](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
        std::unordered_map<char, int> a_map, b_map;

        for (auto &c : a.first)
            a_map[c]++;

        for (auto &c : b.first)
            b_map[c]++;

        // Check if any jokers are in the hand. If so, remove them from the map.
        int a_joker = 0, b_joker = 0;
        if (a_map.find(joker) != a_map.end()) {
            a_joker = a_map[joker];
            a_map.erase(joker);
        }
        if (b_map.find(joker) != b_map.end()) {
            b_joker = b_map[joker];
            b_map.erase(joker);
        }

        int n_a = a_map.size(), n_b = b_map.size();

        if (n_a != n_b) {
            if (n_a + n_b == 1)
                // Here's a special case where one of the hands are all jokers
                // and the other is a Five of a kind.
                return a.first < b.first;
            else
                return n_a > n_b;
        }

        if (n_a != 2 && n_a != 3)
            return a.first < b.first;

        int a_max = 0, b_max = 0;
        for (auto &p : a_map)
            a_max = std::max(a_max, p.second);
        for (auto &p : b_map)
            b_max = std::max(b_max, p.second);

        // Treat jokers as the maximum number of cards.
        a_max += a_joker;
        b_max += b_joker;

        if (a_max != b_max)
            return a_max < b_max;
        else
            return a.first < b.first;
    };

    std::sort(cards.begin(), cards.end(), cmp);

    long long winnings = 0;
    int rank = 1;
    for (auto &card : cards) {
        winnings += (rank++) * card.second;
    }

    std::cout << "Part 2: " << winnings << std::endl;
}

int main() {
    fs::current_path(fs::absolute(fs::path(__FILE__).remove_filename()));
    part1();
    part2();
    return 0;
}