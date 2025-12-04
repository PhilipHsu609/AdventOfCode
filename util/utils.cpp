#include "utils.hpp"

namespace utils {

void set_working_dir(const char *file_path) {
    fs::current_path(fs::absolute(fs::path(file_path).parent_path()));
}

std::string read_file(const fs::path &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "File not found: " << path.string() << std::endl;
        std::exit(1);
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

std::vector<std::string> read_lines(const fs::path &path) {
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

} // namespace utils
