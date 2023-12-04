#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <numeric>

constexpr std::size_t winning_numbers_offset = 10;
constexpr std::size_t winning_numbers_count = 10;
constexpr std::size_t scratched_numbers_offset = 42;
constexpr std::size_t scratched_numbers_count = 25;

int parse_number(std::string const& str, std::size_t offset) {
    return (str[offset] == ' ' ? 0 : (str[offset] - '0') * 10) + (str[offset + 1] - '0');
}

int main() {
    std::ifstream file("input");

    std::vector<std::size_t> matching_numbers_list;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line.empty()) {
            continue;
        }

        std::unordered_set<int> winning_numbers;

        for (std::size_t i = 0; i < winning_numbers_count; i++) {
            int winning_number = parse_number(line, winning_numbers_offset + 3 * i);
            winning_numbers.insert(winning_number);
        }

        std::size_t matching_numbers = 0;

        for (std::size_t i = 0; i < scratched_numbers_count; i++) {
            int scratched_number = parse_number(line, scratched_numbers_offset + 3 * i);
            if (winning_numbers.contains(scratched_number)) {
                matching_numbers++;
            }
        }

        matching_numbers_list.push_back(matching_numbers);
    }

    std::vector<int> card_counts(matching_numbers_list.size(), 1);
    for (std::size_t i = 0; i < card_counts.size(); i++) {
        for (std::size_t j = 1; i + j < card_counts.size() && j <= matching_numbers_list[i]; j++) {
            card_counts[i + j] += card_counts[i];
        }
    }

    std::cout << std::accumulate(card_counts.cbegin(), card_counts.cend(), 0) << std::endl;

}
