#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int constexpr first_label = 0;
int constexpr last_label = 17575;
std::size_t constexpr label_space_size = 17576;

int label_to_int(char const* label) {
    return (label[0] - 'A') * 676 + (label[1] - 'A') * 26 + (label[2] - 'A');
}

bool last_letter_is(int label, char letter) {
    return label % 26 == (letter - 'A');
}

std::pair<std::vector<int>, std::vector<int>> numbers_of_steps(std::vector<int>& current_labels, std::string const& instructions, std::unordered_map<int, std::pair<int, int>> const& nodes) {
    std::vector<int> init_length(current_labels.size());
    std::vector<int> cycle_length(current_labels.size());

    for (std::size_t i = 0; i < current_labels.size(); i++) {
        int& current_label = current_labels[i];
        std::size_t required_steps = 0;
        int end_found_count = 0;

        for (std::size_t j = 0; end_found_count < 2; j = (j + 1) % instructions.size()) {
            required_steps++;

            if (instructions[j] == 'L') {
                current_label = nodes.at(current_label).first;
            } else {
                current_label = nodes.at(current_label).second;
            }

            if (last_letter_is(current_label, 'Z')) {
                if (end_found_count == 0) {
                    init_length[i] = required_steps;
                } else {
                    cycle_length[i] = required_steps - init_length[i];
                }
                end_found_count++;
            }
        }
    }

    return {init_length, cycle_length};
}

int main() {
    std::ifstream file("input");

    std::string instructions;
    std::unordered_map<int, std::pair<int, int>> nodes;
    std::vector<int> current_labels;

    // get instructions
    std::getline(file, instructions);

    {
        std::string line;

        // skip first empty line
        std::getline(file, line);

        // get instructions
        while (std::getline(file, line)) {
            int from = label_to_int(&line[0]);
            int left = label_to_int(&line[7]);
            int right = label_to_int(&line[12]);

            nodes.insert({from, {left, right}});

            if (last_letter_is(from, 'A')) {
                current_labels.push_back(from);
            }
        }
    }

    auto [init, loop] = numbers_of_steps(current_labels, instructions, nodes);
    std::cout << "init: ";
    for (int i : init) {
        std::cout << i << ' ';
    }
    std::cout << "\nloop: ";
    for (int i : loop) {
        std::cout << i << ' ';
    }
    std::cout << "\nIf init and loop are the same, take the LCM" << std::endl;

}
