#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

int main() {
    std::ifstream file("input");

    std::vector<std::string> image;
    std::vector<std::size_t> empty_lines_until; //empty_lines_until[i] is the amount of empty lines between 0 and i (included)
    std::vector<std::size_t> empty_columns_until; // same but for columns
    std::vector<std::pair<std::size_t, std::size_t>> galaxies;

    {
        std::string line;
        std::size_t empty_lines = 0;
        std::size_t i = 0;

        while (std::getline(file, line) && !line.empty()) {
            image.push_back(line);

            bool line_is_empty = true;

            for (std::size_t j = 0; j < line.size(); j++) {
                if (line[j] == '#') {
                    line_is_empty = false;
                    galaxies.push_back({i, j});
                }
            }

            if (line_is_empty) {
                empty_lines++;
            }

            empty_lines_until.push_back(empty_lines);

            i++;
        }
    }

    std::size_t lines = image.size();
    std::size_t columns = image[0].size();

    {
        std::size_t empty_columns = 0;

        for (std::size_t j = 0; j < columns; j++) {
            bool column_is_empty = true;

            for (std::size_t i = 0; i < lines; i++) {
                if (image[i][j] == '#') {
                    column_is_empty = false;
                    break;
                }
            }

            if (column_is_empty) {
                empty_columns++;
            }

            empty_columns_until.push_back(empty_columns);
        }
    }

    std::size_t distances_sum = 0;
    for (std::size_t k = 0; k < galaxies.size(); k++) {
        for (std::size_t l = k + 1; l < galaxies.size(); l++) {
            auto [min_i, max_i] = std::minmax(galaxies[k].first, galaxies[l].first);
            auto [min_j, max_j] = std::minmax(galaxies[k].second, galaxies[l].second);

            distances_sum += (max_i - min_i) + (max_j - min_j) + (empty_lines_until[max_i] - empty_lines_until[min_i]) + (empty_columns_until[max_j] - empty_columns_until[min_j]);
        }
    }

    std::cout << distances_sum << std::endl;
}
