#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

constexpr std::size_t race_count = 4;

int main() {
    std::ifstream file("input");

    std::string dummy;
    int times[race_count];
    int distances[race_count];

    // Read file
    file >> dummy;
    for (std::size_t i = 0; i < race_count; i++) {
        file >> times[i];
    }
    file >> dummy;
    for (std::size_t i = 0; i < race_count; i++) {
        file >> distances[i];
    }

    // t_press * (t_total - t_press) > d_max
    // - t_press^2 + t_total * t_press - d_max > 0
    // delta = t_total^2 - 4 * d_max

    int total_product = 1;

    for (std::size_t i = 0; i < race_count; i++) {
        double delta = times[i] * times[i] - 4 * distances[i];

        if (delta <= 0) {
            // No way to beat the best distance
            total_product = 0;
        } else {
            double first_solution = 0.5 * (times[i] - std::sqrt(delta));
            double second_solution = 0.5 * (times[i] + std::sqrt(delta));

            int smallest_winning_time = std::max(0, (int) std::floor(first_solution + 1));
            int biggest_winning_time = std::min(times[i], (int) std::ceil(second_solution - 1));

            total_product *= biggest_winning_time - smallest_winning_time + 1;
        }
    }

    std::cout << total_product << std::endl;
}
