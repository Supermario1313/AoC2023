#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cctype>

constexpr std::size_t race_count = 4;

int main() {
    std::ifstream file("input");

    std::string line;

    long distance = 0;
    long time = 0;

    // Read file
    std::getline(file, line);
    for (char c : line) {
        if (std::isdigit(c)) {
            time = 10 * time + (c - '0');
        }
    }
    std::getline(file, line);
    for (char c : line) {
        if (std::isdigit(c)) {
            distance = 10 * distance + (c - '0');
        }
    }

    // t_press * (t_total - t_press) > d_max
    // - t_press^2 + t_total * t_press - d_max > 0
    // delta = t_total^2 - 4 * d_max

    double delta = time * time - 4 * distance;
    long ways_to_win;

    if (delta <= 0) {
        // No way to beat the best distance
        ways_to_win = 0;
    } else {
        double first_solution = 0.5 * (time - std::sqrt(delta));
        double second_solution = 0.5 * (time + std::sqrt(delta));

        long smallest_winning_time = std::max(0L, (long) std::floor(first_solution + 1));
        long biggest_winning_time = std::min(time, (long) std::ceil(second_solution - 1));

        ways_to_win = biggest_winning_time - smallest_winning_time + 1;
    }

    std::cout << ways_to_win << std::endl;
}
