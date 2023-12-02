#include <regex>
#include <fstream>
#include <iostream>
#include <charconv>
#include <string>
#include <string_view>
#include <limits>
#include <algorithm>

std::regex const game_regex(R"(Game (\d+): )");
std::regex const draw_regex(R"((\d+) (red|green|blue))");

int main() {
    std::ifstream file("input");
    unsigned long sum = 0;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        unsigned long game_index;
        unsigned long maxred = 0;
        unsigned long maxgreen = 0;
        unsigned long maxblue = 0;

        std::smatch game_match;

        if (!std::regex_search(line, game_match, game_regex)) {
            continue;
        }

        std::from_chars(std::to_address(game_match[1].first), std::to_address(game_match[1].second), game_index);

        std::sregex_iterator draw_iterator_begin(game_match[1].second, line.cend(), draw_regex);
        std::sregex_iterator draw_iterator_end;

        for (std::sregex_iterator i = draw_iterator_begin; i != draw_iterator_end; i++) {
            std::smatch match = *i;

            unsigned long amount;
            std::string_view color(match[2].first, match[2].second);

            std::from_chars(std::to_address(match[1].first), std::to_address(match[1].second), amount);

            if (color == "red") {
                maxred = std::max(maxred, amount);
            } else if (color == "green") {
                maxgreen = std::max(maxgreen, amount);
            } else if (color == "blue") {
                maxblue = std::max(maxblue, amount);
            }
        }

        sum += maxred * maxgreen * maxblue;
    }

    std::cout << sum << std::endl;
}
