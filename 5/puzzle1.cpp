#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <limits>

struct value_range {
    long value;
    long range;
};

void fill_seed_list(std::vector<long>& seeds, std::ifstream& file) {
    std::string line;
    std::string dummy;

    // Read first line (seed list)
    std::getline(file, line);
    std::istringstream seeds_stream(line);

    // Skip line prefix
    seeds_stream >> dummy;

    // Read seed values and add them to the list
    long seed;

    while(seeds_stream >> seed) {
        seeds.push_back(seed);
    }

    // Skip empty line after the seed list
    std::getline(file, line);
}

void fill_map(std::map<long, value_range>& map, std::ifstream& file) {
    std::string line;

    // Skip title line
    std::getline(file, line);

    // Read first map entry
    std::getline(file, line);

    // Read map
    while (!line.empty()) {
        long key;
        long value;
        long range;

        // Read key, value and range
        std::istringstream map_entry_stream(line);
        map_entry_stream >> value >> key >> range;

        // Add entry to map
        map.insert({key, { .value = value, .range = range }});

        // Read next map entry
        std::getline(file, line);
    }
}

std::map<long, value_range>::iterator biggest_key_smaller_than(std::map<long, value_range>& map, long key) {
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

long get_value_from_map(std::map<long, value_range>& map, long key) {
    // Get the entry associated to the biggest key which is smaller or equal to the key argument
    auto map_entry = biggest_key_smaller_than(map, key);

    // If the found entry is an iterator to the end or the key is not within the range, return the key
    if (map_entry == map.end() || key > map_entry->first + map_entry->second.range - 1) {
        return key;
    }

    // Otherwise, return the correct mapped value
    return map_entry->second.value + key - map_entry->first;

}

int main() {
    // The input file
    std::ifstream file("input");

    // Program data structures
    std::vector<long> seeds;
    std::map<long, value_range> seed_to_soil;
    std::map<long, value_range> soil_to_fertilizer;
    std::map<long, value_range> fertilizer_to_water;
    std::map<long, value_range> water_to_light;
    std::map<long, value_range> light_to_temperature;
    std::map<long, value_range> temperature_to_humidity;
    std::map<long, value_range> humidity_to_location;

    // Read file
    fill_seed_list(seeds, file);
    fill_map(seed_to_soil, file);
    fill_map(soil_to_fertilizer, file);
    fill_map(fertilizer_to_water, file);
    fill_map(water_to_light, file);
    fill_map(light_to_temperature, file);
    fill_map(temperature_to_humidity, file);
    fill_map(humidity_to_location, file);

    // Get smallest location
    long min_location = std::numeric_limits<long>::max();
    for (auto seed : seeds) {
        long soil = get_value_from_map(seed_to_soil, seed);
        long fertilizer = get_value_from_map(soil_to_fertilizer, soil);
        long water = get_value_from_map(fertilizer_to_water, fertilizer);
        long light = get_value_from_map(water_to_light, water);
        long temperature = get_value_from_map(light_to_temperature, light);
        long humidity = get_value_from_map(temperature_to_humidity, temperature);
        long location = get_value_from_map(humidity_to_location, humidity);

        if (location < min_location) {
            min_location = location;
        }
    }

    std::cout << min_location << std::endl;


}
