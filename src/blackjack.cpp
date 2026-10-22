#include "blackjack.h"

#include <iostream>
#include <random>
#include <string>

#include "card.h"
#include "deck.h"

namespace {

/// The first five cards of a deck as text. The deck is passed by value on
/// purpose: the copy is dealt from, and the caller's deck is left alone.
std::string top_five(Deck deck) {
  std::string text{};
  for (int i{0}; i < 5 && !deck.empty(); ++i) {
    if (i > 0) {
      text += ' ';
    }
    text += deck.deal().to_string();
  }
  return text;
}

} // namespace

void play_blackjack() {
  std::random_device device{};
  std::mt19937 gen{device()};

  std::cout << "\n--- Blackjack ---\n";

  Deck deck;
  if (deck.empty()) {
    std::cout << "The deck is empty. Write Deck() in deck.cpp first.\n";
    return;
  }

  std::cout << "Cards in a new deck: " << deck.remaining() << "\n";

  deck.shuffle(gen);

  const std::string before{top_five(deck)};

  Deck copy = deck;

  copy.shuffle(gen);
  const std::string copy_top{top_five(copy)};
  const std::string after{top_five(deck)};

  std::cout
      << "Original, top five:                 " << before << "\n"
      << "Shuffled copy, top five:            " << copy_top << "\n"
      << "Original after shuffling the copy:  " << after << "\n"
      << (before == after
              ? "The original did not change.\n"
              : "The original CHANGED. The copy shares memory with it!\n");
}
