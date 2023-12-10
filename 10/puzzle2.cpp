#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

enum class direction_t {
    up, down, left, right
};

direction_t opposite_directon(direction_t direction) {
    switch (direction) {
        case direction_t::down:
            return direction_t::up;
        case direction_t::up:
            return direction_t::down;
        case direction_t::left:
            return direction_t::right;
        case direction_t::right:
        default:
            return direction_t::left;
    }
}

void increment_coordinates(std::size_t& i, std::size_t& j, direction_t direction) {
    switch (direction) {
        case direction_t::down:
            i++;
            break;
        case direction_t::up:
            i--;
            break;
        case direction_t::left:
            j--;
            break;
        case direction_t::right:
            j++;
            break;
    }
}

class pipe_t {
    direction_t directions[2];
    bool ground;
    bool loop_boundary;
public:
    pipe_t() : ground(true), loop_boundary(false) {}

    pipe_t(char c) : ground(false), loop_boundary(false) {
        switch (c) {
            case '|':
                directions[0] = direction_t::up;
                directions[1] = direction_t::down;
                break;
            case '-':
                directions[0] = direction_t::left;
                directions[1] = direction_t::right;
                break;
            case 'L':
                directions[0] = direction_t::up;
                directions[1] = direction_t::right;
                break;
            case 'J':
                directions[0] = direction_t::up;
                directions[1] = direction_t::left;
                break;
            case '7':
                directions[0] = direction_t::down;
                directions[1] = direction_t::left;
                break;
            case 'F':
                directions[0] = direction_t::down;
                directions[1] = direction_t::right;
                break;
            default:
                std::cout << "Invalid character: '" << c << '\'' << std::endl;
                std::exit(-1);
        }
    }

    bool has_direction(direction_t direction) const {
        return !ground && (direction == directions[0] || direction == directions[1]);
    }

    direction_t get_arbitraty_direction() const {
        return directions[0];
    }

    // UB if ground == true
    direction_t other_end(direction_t direction) const {
        return direction == directions[0] ? directions[1] : directions[0];
    }

    // We assume that the current position is only connected to 2 pipes
    void infer_directions(std::vector<std::vector<pipe_t>> const& maze, std::size_t i, std::size_t j) {
        ground = false;
        int direction_to_set = 0;

        if (i > 0 && maze[i - 1][j].has_direction(direction_t::down)) {
            directions[direction_to_set] = direction_t::up;
            direction_to_set++;
        }

        if (j > 0 && maze[i][j - 1].has_direction(direction_t::right)) {
            directions[direction_to_set] = direction_t::left;
            direction_to_set++;
        }

        if (i < maze.size() - 1 && maze[i + 1][j].has_direction(direction_t::up)) {
            directions[direction_to_set] = direction_t::down;
            direction_to_set++;
        }

        if (j < maze[i].size() - 1 && maze[i][j + 1].has_direction(direction_t::left)) {
            directions[direction_to_set] = direction_t::right;
            direction_to_set++;
        }
    }

    bool& on_loop_boundary() {
        return loop_boundary;
    }
};

int main() {
    std::ifstream file("input");

    // Read file
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line) && line.size() > 0) {
        lines.push_back(line);
    }
    std::size_t num_lines = lines.size();
    std::size_t num_columns = lines[0].size();

    // Make maze
    std::vector<std::vector<pipe_t>> maze(num_lines, std::vector<pipe_t>(num_columns));
    std::size_t start_i = 0;
    std::size_t start_j = 0;
    for (std::size_t i = 0; i < num_lines; i++) {
        for (std::size_t j = 0; j < num_columns; j++) {
            if (lines[i][j] == 'S') {
                start_i = i;
                start_j = j;
            } else if (lines[i][j] == '.') {
                // Do nothing
            } else {
                maze[i][j] = pipe_t(lines[i][j]);
            }
        }
    }

    // Determine starting_point directions
    maze[start_i][start_j].infer_directions(maze, start_i, start_j);

    // Check which tiles are on the loop boundary
    std::size_t i = start_i;
    std::size_t j = start_j;
    direction_t incoming_direction = maze[start_i][start_j].get_arbitraty_direction();

    do {
        direction_t next_direction = maze[i][j].other_end(incoming_direction);
        increment_coordinates(i, j, next_direction);
        incoming_direction = opposite_directon(next_direction);
        maze[i][j].on_loop_boundary() = true;
    } while (i != start_i || j != start_j);

    // Count tiles inside loop
    std::size_t count = 0;

    for (std::size_t i = 0; i < num_lines; i++) {
        bool inside_loop = false;
        for (std::size_t j = 0; j < num_columns; j++) {
            if (maze[i][j].on_loop_boundary()) {
                bool boundary_start_has_up = maze[i][j].has_direction(direction_t::up);
                bool boundary_start_has_down = maze[i][j].has_direction(direction_t::down);

                while (maze[i][j].has_direction(direction_t::right)) {
                    j++;
                }

                if ((boundary_start_has_down && maze[i][j].has_direction(direction_t::up)) || (boundary_start_has_up && maze[i][j].has_direction(direction_t::down))) {
                    inside_loop = !inside_loop;
                }
            } else if (inside_loop) {
                count++;
            }
        }
    }

    std::cout << count << std::endl;
}
