#include <regex>
#include <fstream>
#include <iostream>

std::regex const first_digit(R"(\D*(\d).*)");
std::regex const last_digit(R"(.*(\d)\D*)");

int main() {
    std::ifstream file("input");
    int sum = 0;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        std::smatch match_results;
        int calibration = 0;

        if (!std::regex_match(line, match_results, first_digit)) {
            continue;
        }
        calibration += 10 * (match_results[1].str()[0] - '0'); // 0th character of 1st submatch

        if (!std::regex_match(line, match_results, last_digit)) {
            continue;
        }
        calibration += match_results[1].str()[0] - '0'; // 0th character of 1st submatch

        sum += calibration;
    }

    std::cout << sum << std::endl;
}
