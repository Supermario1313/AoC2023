#include <regex>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <string_view>

std::regex const digit_regex(R"(\d|zero|one|two|three|four|five|six|seven|eight|nine)");

std::unordered_map<std::string_view, int> digit_to_int {
    {"0", 0},
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
};

int main() {
    std::ifstream file("input");
    int sum = 0;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        std::string::const_iterator search_begin = line.cbegin();
        std::smatch match;

        int first_digit = -1;
        int last_digit;

        while (regex_search(search_begin, line.cend(), match, digit_regex)) {
            std::string_view digit_str = std::string_view(match[0].first, match[0].second);
            int digit = digit_to_int[digit_str];

            if (first_digit == -1) {
                first_digit = digit;
            }

            last_digit = digit;

            search_begin = match[0].first + 1;
        }

        if (first_digit != -1) {
            int calibration = first_digit * 10 + last_digit;

            sum += calibration;
        }
    }

    std::cout << sum << std::endl;
}
