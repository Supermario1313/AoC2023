#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::ifstream file("input");
    std::string line;
    int interpolated_sum = 0;

    while (std::getline(file, line)) {
        if (line.size() > 0) {
            std::istringstream int_stream(line);
            int number;

            std::vector<std::vector<int>> sequences(1);
            bool only_zeros = true;

            while (int_stream >> number) {
                sequences.back().push_back(number);
                if (number != 0) {
                    only_zeros = false;
                }
            }

            while (!only_zeros) {
                sequences.emplace_back(sequences.back().size() - 1);

                std::vector<int>& current_sequence = sequences.back();
                std::vector<int>& previous_sequence = *std::prev(sequences.end(), 2);

                only_zeros = true;
                for (std::size_t i = 0; i < current_sequence.size(); i++) {
                    current_sequence[i] = previous_sequence[i + 1] - previous_sequence[i];
                    if (current_sequence[i] != 0) {
                        only_zeros = false;
                    }
                }
            }

            sequences.back().push_back(0);
            for (std::size_t i = sequences.size() - 2; i < sequences.size() - 1; i--) {
                sequences[i].push_back(sequences[i].back() + sequences[i + 1].back());
            }

            interpolated_sum += sequences.front().back();
        }
    }

    std::cout << interpolated_sum << std::endl;
}
