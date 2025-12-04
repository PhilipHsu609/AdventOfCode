#pragma once

#include <charconv>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace utils {
namespace fs = std::filesystem;

class Timer {
  public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    void reset() { start_ = std::chrono::high_resolution_clock::now(); }

    double elapsed() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }

    double elapsed_and_reset() {
        auto e = elapsed();
        reset();
        return e;
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

inline void set_working_dir(const char *file_path) {
    fs::current_path(fs::absolute(fs::path(file_path).parent_path()));
}

inline std::string read_file(const fs::path &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "File not found: " << path.string() << std::endl;
        std::exit(1);
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

inline std::vector<std::string> read_lines(const fs::path &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "File not found: " << path.string() << std::endl;
        std::exit(1);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

template <typename T = std::string>
inline std::vector<T> split(const std::string &s, char delim) {
    std::vector<T> result;
    std::stringstream ss(s);
    std::string item;

    auto cvt = [](const std::string &str) -> T {
        if constexpr (std::is_integral_v<T>) {
            T value;
            std::from_chars(str.data(), str.data() + str.size(), value);
            return value;
        } else {
            return str;
        }
    };

    while (std::getline(ss, item, delim)) {
        result.emplace_back(cvt(item));
    }
    return result;
}
} // namespace utils