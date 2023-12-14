#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <optional>

enum class orientation_t {
    horizontal, vertical
};

bool columns_equal(std::vector<std::string> const& pattern, std::size_t j_1, std::size_t j_2) {
    for (std::size_t i = 0; i < pattern.size(); i++) {
        if (pattern[i][j_1] != pattern[i][j_2]) {
            return false;
        }
    }

    return true;
}

std::optional<std::pair<orientation_t, std::size_t>> handle_pattern(std::ifstream& file) {
    std::vector<std::string> pattern;

    {
        std::string line;
        while (std::getline(file, line) && !line.empty()) {
            pattern.push_back(line);
        }

        if (pattern.size() == 0) {
            return std::nullopt;
        }
    }

    {
        std::size_t mirrored_i = 0;
        std::size_t lines_above_reflection = 1;
        for (std::size_t i = 1; i < pattern.size(); i++) {
            if (pattern[mirrored_i] == pattern[i]) {
                mirrored_i--;
            } else {
                mirrored_i = i;
                lines_above_reflection = i + 1;
            }

            if (mirrored_i >= pattern.size()) {
                return std::make_optional(std::make_pair(orientation_t::horizontal, lines_above_reflection));
            }
        }

        if (mirrored_i != pattern.size() - 1) {
            return std::make_optional(std::make_pair(orientation_t::horizontal, lines_above_reflection));
        }
    }

    std::size_t const columns = pattern[0].size();

    {
        std::size_t mirrored_j = 0;
        std::size_t columns_before_reflection = 1;
        for (std::size_t j = 1; j < columns; j++) {
            if (columns_equal(pattern, j, mirrored_j)) {
                mirrored_j--;
            } else {
                mirrored_j = j;
                columns_before_reflection = j + 1;
            }

            if (mirrored_j >= columns) {
                return std::make_optional(std::make_pair(orientation_t::vertical, columns_before_reflection));
            }
        }

        if (mirrored_j != columns - 1) {
            return std::make_optional(std::make_pair(orientation_t::vertical, columns_before_reflection));
        }
    }

    std::cout << "Should never happen" << std::endl;
    std::exit(1);
}

int main() {
    std::ifstream file("input");

    std::size_t horizontal = 0;
    std::size_t vertical = 0;

    while (auto ret = handle_pattern(file)) {
        auto [orientation, count] = ret.value();

        if (orientation == orientation_t::horizontal) {
            horizontal += count;
        } else {
            vertical += count;
        }
    }

    std::cout << horizontal * 100 + vertical << std::endl;
}
