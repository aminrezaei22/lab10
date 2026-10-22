// deck_bugs.cpp - three memory bugs in a small Deck-like class.
//
// This file does not use your Deck. It has its own RawDeck so the bugs are the
// same for everyone.
//
// Build with AddressSanitizer:
//   g++ -std=c++20 -Wall -Wextra -g -fsanitize=address gdb/deck_bugs.cpp
//   src/card.cpp -Isrc -o deck_bugs
// Run one bug at a time:
//   ./deck_bugs 1      (a leak)
//   ./deck_bugs 2      (a shallow copy)
//   ./deck_bugs 3      (a double delete)

#include <iostream>
#include <string>

#include "card.h"

class RawDeck {
public:
  RawDeck() : cards_{new Card[52]}, size_{52} {
    for (int i = 0; i < size_; ++i) {
      cards_[i] = Card{i % 13 + 1, static_cast<Suit>(i / 13)};
    }
  }

  ~RawDeck() { delete[] cards_; }

  const Card &at(int index) const { return cards_[index]; }

  int size() const { return size_; }

private:
  Card *cards_;
  int size_;
};

// Bug 1
void deal_a_hand() {
  Card *hand = new Card[52];
  hand[0] = Card{9, Suit::clubs};
  std::cout << "Top card: " << hand[0].to_string() << "\n";
}

// Bug 2
void copy_a_deck() {
  RawDeck original;
  RawDeck backup = original;
  std::cout << "Backup top card: " << backup.at(0).to_string() << "\n";
}

// Bug 3
void clear_a_table() {
  Card *table = new Card[5];
  table[0] = Card{12, Suit::hearts};
  std::cout << "Table card: " << table[0].to_string() << "\n";
  delete[] table;
  std::cout << "Table cleared.\n";
  delete[] table;
}

int main(int argc, char *argv[]) {
  const std::string which = argc > 1 ? argv[1] : "";
  if (which == "1") {
    deal_a_hand();
  } else if (which == "2") {
    copy_a_deck();
  } else if (which == "3") {
    clear_a_table();
  } else {
    std::cout << "Usage: ./deck_bugs 1|2|3\n";
    return 1;
  }
  return 0;
}
