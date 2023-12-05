#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <limits>
#include <unordered_map>

constexpr bool debug = false;

struct value_range {
    long base;
    long size;
};

void fill_seed_list(std::vector<value_range>& seeds, std::ifstream& file) {
    std::string line;
    std::string dummy;

    // Read first line (seed list)
    std::getline(file, line);
    std::istringstream seeds_stream(line);

    // Skip line prefix
    seeds_stream >> dummy;

    // Read seed values and ranges and add them to the list
    value_range seed_range;

    while(seeds_stream >> seed_range.base >> seed_range.size) {
        seeds.push_back(seed_range);
    }

    // Skip empty line after the seed list
    std::getline(file, line);
}

bool fill_map(std::map<long, value_range>& map, std::ifstream& file) {
    std::string line;

    // Skip title line, return false if we reached EOF
    if (!std::getline(file, line)) {
        return false;
    }

    // Read first map entry
    std::getline(file, line);

    // Read map
    while (!line.empty()) {
        long key;
        value_range value;

        // Read key, value and size
        std::istringstream map_entry_stream(line);
        map_entry_stream >> value.base >> key >> value.size;

        // Add entry to map
        map.insert({key, value});

        // Read next map entry
        std::getline(file, line);
    }

    return true;
}

std::map<long, value_range>::const_iterator biggest_key_smaller_than(std::map<long, value_range> const& map, long key) {
    // Get the entry associated to the smallest key which is greater or equal to the key argument
    auto map_entry = map.upper_bound(key);

    // If the found entry is not the iterator to the end and its key is equal to the key argument, return this entry.
    if (map_entry != map.end() && map_entry->first == key) {
        return map_entry;
    }

    // If the found entry is the iterator to the beginning, return the iterator to the end.
    if (map_entry == map.begin()) {
        return map.end();
    }

    // Otherwise, return the predecessor of the entry
    return std::prev(map_entry);
}

long get_value_from_map(std::map<long, value_range> const& map, long key) {
    // Get the entry associated to the biggest key which is smaller or equal to the key argument
    auto map_entry = biggest_key_smaller_than(map, key);

    // If the found entry is an iterator to the end or the key is not within the range, return the key
    if (map_entry == map.end() || key > map_entry->first + map_entry->second.size - 1) {
        return key;
    }

    // Otherwise, return the correct mapped value
    return map_entry->second.base + key - map_entry->first;
}

std::vector<value_range> map_ranges(std::map<long, value_range> const& map, std::vector<value_range> const& range_list) {
    std::vector<value_range> return_value;

    for (auto range : range_list) {
        while (range.size > 0) {
            value_range current_destination;

            // Get the entry associated to the biggest key which is smaller or equal to the base value of the current range
            auto entry = biggest_key_smaller_than(map, range.base);

            if (entry != map.end() && range.base - entry->first < entry->second.size) {
                // If the base value of the current range is within a key range, then it maps to the base value of the corresponding value range plus the distance between the base value of the current range and the base value of the key range.
                current_destination.base = entry->second.base + range.base - entry->first;

                // The size of the destination range is whichever is the smallest between the size of the current range and the remaining size of the value range of the entry
                current_destination.size = std::min(range.size, entry->second.size + entry->first - range.base);
            } else {
                // Otherwise, if it's not in any key range, The base value maps to itself
                current_destination.base = range.base;

                // The size of the destination range is whichever is the smallest between the current size and distance between the current base (included) and the base of the next key range if it exists(excluded)
                auto next_entry = entry == map.end() ? map.begin() : std::next(entry);

                if (next_entry != map.end()) {
                    current_destination.size = std::min(range.size, next_entry->second.base - range.base);
                } else {
                    current_destination.size = range.size;
                }
            }

            // Add the destination range to the list and adjust the current range
            return_value.push_back(current_destination);
            range.base += current_destination.size;
            range.size -= current_destination.size;
        }
    }

    return return_value;
}

int main() {
    // The input file
    std::ifstream file("input");

    // Program data structures
    std::vector<value_range> seed_ranges;
    std::vector<std::map<long, value_range>> maps;

    // Read file
    fill_seed_list(seed_ranges, file);

    std::map<long, value_range> current_map;
    while (fill_map(current_map, file)) {
        maps.push_back(std::move(current_map));
        current_map.clear();
    }

    // Get smallest location
    long min_location = std::numeric_limits<long>::max();
    for (auto seed_range : seed_ranges) {
        if constexpr (debug) {
            std::cout << "Handling " << seed_range.base << " +" << seed_range.size << std::endl;
        }

        std::vector<value_range> location_ranges { seed_range };

        // Apply all maps successively to the accumulator (named location here)
        for (std::size_t i = 0; i < maps.size(); i++) {
            if constexpr (debug) {
                std::cout << "Step " << i << std::flush;
            }

            location_ranges = map_ranges(maps[i], location_ranges);

            if constexpr (debug) {
                std::cout << " => Obtained " << location_ranges.size() << " ranges:" << std::endl;

                for (auto range : location_ranges) {
                    std::cout << range.base << " +" << range.size << std::endl;
                }
            }
        }

        for (auto range : location_ranges) {
            if (range.base < min_location) {
                min_location = range.base;
            }
        }
    }

    if constexpr (debug) {
        std::cout << std::endl << "Solution: ";
    }

    std::cout << min_location << std::endl;
}
