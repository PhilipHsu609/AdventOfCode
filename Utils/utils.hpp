#include <charconv>    // from_chars
#include <sstream>     // stringstream
#include <string>      // string, getline
#include <type_traits> // is_integral
#include <vector>      // vector

namespace Utils {
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
} // namespace Utils