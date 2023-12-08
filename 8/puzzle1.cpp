#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>

int label_to_int(char const* label) {
    return (label[0] - 'A') * 676 + (label[1] - 'A') * 26 + (label[2] - 'A');
}

int constexpr first_label = 0;
int constexpr last_label = 17575;
std::size_t constexpr label_space_size = 17576;

int main() {
    std::ifstream file("input");

    std::string instructions;
    std::unordered_map<int, std::pair<int, int>> nodes;

    // get instructions
    std::getline(file, instructions);

    {
        std::string line;

        // skip first empty line
        std::getline(file, line);

        // get instructions
        while (std::getline(file, line)) {
            nodes.insert({label_to_int(&line[0]), {label_to_int(&line[7]), label_to_int(&line[12])}});
        }
    }

    // Traverse nodes until we reach "ZZZ"
    std::size_t required_steps = 0;
    int current_label = first_label;
    for (std::size_t i = 0; current_label != last_label; i = (i + 1) % instructions.size()) {
        required_steps++;
        if (instructions[i] == 'L') {
            current_label = nodes[current_label].first;
        } else {
            current_label = nodes[current_label].second;
        }
    }

    std::cout << required_steps << std::endl;
}
