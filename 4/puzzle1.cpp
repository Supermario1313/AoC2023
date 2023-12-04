#include <fstream>
#include <iostream>
#include <unordered_set>

constexpr int winning_numbers_offset = 10;
constexpr int winning_numbers_count = 10;
constexpr int scratched_numbers_offset = 42;
constexpr int scratched_numbers_count = 25;

int parse_number(std::string const& str, int offset) {
    return (str[offset] == ' ' ? 0 : (str[offset] - '0') * 10) + (str[offset + 1] - '0');
}

void increase_card_value(int& card_value) {
    if (card_value == 0) {
        card_value++;
    } else {
        card_value *= 2;
    }
}

int main() {
    std::ifstream file("input");
    int total_points = 0;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line.empty()) {
            continue;
        }

        std::unordered_set<int> winning_numbers;
        int card_value = 0;

        for (int i = 0; i < winning_numbers_count; i++) {
            int winning_number = parse_number(line, winning_numbers_offset + 3 * i);
            winning_numbers.insert(winning_number);
        }


        for (int i = 0; i < scratched_numbers_count; i++) {
            int scratched_number = parse_number(line, scratched_numbers_offset + 3 * i);
            if (winning_numbers.contains(scratched_number)) {
                increase_card_value(card_value);
            }
        }

        total_points += card_value;
    }

    std::cout << total_points << std::endl;
}
