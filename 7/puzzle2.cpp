#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <utility>
#include <iterator>
#include <unordered_map>

constexpr int hand_size = 5;

enum class hand_type {
    high_card = 0, one_pair, two_pair, three_of_a_kind, full_house, four_of_a_kind, five_of_a_kind
};

struct camel_hand {
    char cards[hand_size + 1];
    hand_type type;
    std::size_t bid;
};

int card_value(char c) {
    if (std::isdigit(c)) {
        return c - '0';
    } else {
        switch (c) {
            case 'T': return 10;
            case 'J': return 1;
            case 'Q': return 12;
            case 'K': return 13;
            case 'A': return 14;
        }
    }

    std::cout << "Should not happen (" << (int) c << ')' << std::endl;
    std::exit(-1); // should never happen
}

bool compare_cards(char const& char_1, char const& char_2) {
    return card_value(char_1) < card_value(char_2);
}

bool compare_hands(camel_hand const& hand_1, camel_hand const& hand_2) {
    if (std::to_underlying(hand_1.type) < std::to_underlying(hand_2.type)) {
        return true;
    } else if (std::to_underlying(hand_1.type) == std::to_underlying(hand_2.type)) {
        return std::lexicographical_compare(std::begin(hand_1.cards), std::prev(std::end(hand_1.cards)), std::begin(hand_2.cards), std::prev(std::end(hand_2.cards)), compare_cards);
    } else {
        return false;
    }
}

hand_type get_hand_type(std::unordered_multimap<std::size_t, char> const& char_from_count) {
    // determine hand type
    if (char_from_count.count(5) == 1) {
        return hand_type::five_of_a_kind;
    } else if (char_from_count.count(4) == 1) {
        return hand_type::four_of_a_kind;
    } else if (char_from_count.count(3) == 1) {
        if (char_from_count.count(2) == 1) {
            return hand_type::full_house;
        } else {
            return hand_type::three_of_a_kind;
        }
    } else if (char_from_count.count(2) == 2) {
        return hand_type::two_pair;
    } else if (char_from_count.count(2) == 1) {
        return hand_type::one_pair;
    } else {
        return hand_type::high_card;
    }
}

int main() {
    std::ifstream file("input");

    std::vector<camel_hand> hands;

    camel_hand hand;
    while (file >> hand.cards >> hand.bid) {
        hand.type = hand_type::high_card;

        for (char joker_substitute : {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'}) {
            std::unordered_map<char, std::size_t> char_count;

            for (int i = 0; i < hand_size; i++) {
                char_count[hand.cards[i] == 'J' ? joker_substitute : hand.cards[i]]++;
            }

            std::unordered_multimap<std::size_t, char> char_from_count;

            for (auto [card, count] : char_count) {
                char_from_count.insert({count, card});
            }

            hand_type hand_type_hypothesis = get_hand_type(char_from_count);
            hand.type = std::to_underlying(hand_type_hypothesis) > std::to_underlying(hand.type) ? hand_type_hypothesis : hand.type;
        }

        // add hand to list
        hands.push_back(hand);
    }

    std::sort(hands.begin(), hands.end(), compare_hands);

    std::size_t total_winnings = 0;

    for (std::size_t i = 0; i < hands.size(); i++) {
        total_winnings += (i + 1) * hands[i].bid;
    }

    std::cout << total_winnings << std::endl;
}
