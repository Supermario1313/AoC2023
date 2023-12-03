#include <string>
#include <fstream>
#include <iostream>
#include <charconv>
#include <vector>
#include <optional>
#include <cctype>

void add_gear_ratio(std::vector<std::string>& lines, std::size_t i, std::size_t j, int& sum) {
    // to_check[k][l] is true if we should check lines[i + k - 1][j + l - 1].
    bool to_check[3][3] {
        {i > 0 && j > 0,                j > 0,                j > 0 && i < lines[j-1].size() - 1},
        {i > 0,                         false,                i < lines[j].size() - 1},
        {i > 0 && j < lines.size() - 1, j < lines.size() - 1, j < lines.size() - 1 && i < lines[j+1].size() - 1}
    };
    int number_count = 0;
    int numbers[2];

    for (std::size_t k = 0; k < 3; k++) {
        for (std::size_t l = 0; l < 3; l++) {
            if (to_check[k][l]) {
                if (std::isdigit(lines[i + k - 1][j + l - 1])) {
                    // We found a neighboring number

                    // If there are already 2 neighboring numbers, return without incrementing the sum
                    if (number_count == 2) {
                        return;
                    }


                    // Otherwise, get the position of the found digit
                    std::size_t digit_line = i + k - 1;
                    std::size_t digit_index = j + l - 1;

                    // Search for the beginning and the ending of the found number and update the to_check array
                    std::size_t number_start = digit_index;
                    std::size_t number_end = digit_index;

                    while (number_start < lines[digit_line].size() && std::isdigit(lines[digit_line][number_start])) { // We don't check for number_start >= 0 because std::size_t is an unsigned type
                        int current_l = number_start - j + 1;
                        if (current_l < 3) {
                            to_check[k][current_l] = false;
                        }

                        number_start--;
                    }
                    number_start++;

                    while (number_end < lines[digit_line].size() && std::isdigit(lines[digit_line][number_end])) {
                        int current_l = number_end - j + 1;
                        if (current_l < 3) {
                            to_check[k][current_l] = false;
                        }
                        number_end++;
                    }

                    // Extract the number to the string and store it in the number array
                    std::from_chars(lines[digit_line].c_str() + number_start, lines[digit_line].c_str() + number_end, numbers[number_count]);
                    number_count++;
                }
            }
        }
    }

    if (number_count == 2) {
        sum += numbers[0] * numbers[1];
    }
}

int main() {
    std::ifstream file("input");
    int sum = 0;

    std::vector<std::string> lines;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        if (line.size() > 0) {
            lines.push_back(line);
        }
    }

    for (std::size_t i = 0; i < lines.size(); i++) {
        for (std::size_t j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == '*') {
                add_gear_ratio(lines, i, j, sum);
            }
        }
    }

    std::cout << sum << std::endl;
}
