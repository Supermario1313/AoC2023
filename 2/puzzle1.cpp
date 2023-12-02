#include <regex>
#include <fstream>
#include <iostream>
#include <charconv>
#include <string>
#include <string_view>

std::regex const game_regex(R"(Game (\d+): )");
std::regex const draw_regex(R"((\d+) (red|green|blue))");

int main() {
    std::ifstream file("input");
    int sum = 0;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        int game_index;

        std::smatch game_match;

        if (!std::regex_search(line, game_match, game_regex)) {
            continue;
        }

        std::from_chars(std::to_address(game_match[1].first), std::to_address(game_match[1].second), game_index);

        std::sregex_iterator draw_iterator_begin(game_match[1].second, line.cend(), draw_regex);
        std::sregex_iterator draw_iterator_end;

        for (std::sregex_iterator i = draw_iterator_begin; i != draw_iterator_end; i++) {
            std::smatch match = *i;

            int amount;
            std::string_view color(match[2].first, match[2].second);

            std::from_chars(std::to_address(match[1].first), std::to_address(match[1].second), amount);

            if ((color == "red" && amount > 12) || (color == "green" && amount > 13) || (color == "blue" && amount > 14)) {
                goto continue_while_loop;
            }
        }

        sum += game_index;

        continue_while_loop: ;
    }

    std::cout << sum << std::endl;
}
