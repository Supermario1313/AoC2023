#include <string>
#include <regex>
#include <fstream>
#include <iostream>
#include <charconv>
#include <vector>

std::regex const number_regex(R"(\d+)");

bool is_symbol(char c) {
    return c != '.' && (c < '0' || c > '9');
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

    std::size_t line_size = lines[0].size();

    for (std::size_t i = 0; i < lines.size(); i++) {
        std::sregex_iterator number_iterator_begin(lines[i].cbegin(), lines[i].cend(), number_regex);
        std::sregex_iterator number_iterator_end;

        for (std::sregex_iterator j = number_iterator_begin; j != number_iterator_end; j++) {
            std::smatch match = *j;
            std::size_t number_start_index = match.position();
            std::size_t number_end_index = match.position() + match.length() - 1;
            int number = 0;

            std::from_chars(lines[i].c_str() + number_start_index, lines[i].c_str() + number_end_index + 1, number);

            // Check if there is a symbol on the line above
            if (i > 0) {
                for (std::size_t k = std::max(number_start_index, (std::size_t) 1) - 1; k <= std::min(number_end_index + 1, line_size - 1); k++) {
                    if (is_symbol(lines[i-1][k])) {
                        sum += number;
                        goto next_number;
                    }
                }
            }

            // Check if the character before or after the number is a symbol
            if ((number_start_index > 0 && is_symbol(lines[i][number_start_index - 1])) || (number_end_index < line_size - 1 && is_symbol(lines[i][number_end_index + 1]))) {
                sum += number;
                goto next_number;
            }

            // Check if there is a symbol on the line below
            if (i < lines.size() - 1) {
                for (std::size_t k = std::max(number_start_index, (std::size_t) 1) - 1; k <= std::min(number_end_index + 1, line_size - 1); k++) {
                    if (is_symbol(lines[i+1][k])) {
                        sum += number;
                        goto next_number;
                    }
                }
            }

            next_number: ;
        }
    }

    std::cout << sum << std::endl;
}
