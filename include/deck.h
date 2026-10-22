#ifndef ARCADE_DECK_H
#define ARCADE_DECK_H

#include <random>

#include "card.h"

/// @file deck.h
/// @brief A deck of 52 cards that owns its memory.

/// The number of cards in a full deck.
inline constexpr int deck_size = 52;

// TODO: Write the class invariant here as a comment.
//  Say what m_cards points to, what m_size is, and what range m_next may have.
//
// TODO: Explain here what goes wrong if operator= omits the self-assignment
//  check (d = d;).

/// A deck of playing cards stored in a heap array.
class Deck {
public:
  /// Make a full, ordered deck: one of each rank in each suit.
  /// TODO: allocate with new[] and fill in all 52 cards.
  Deck();

  /// Free the cards.
  /// TODO:
  ~Deck();

  /// Make an independent copy of another deck.
  /// TODO: A deep copy, not a copy of the pointer.
  Deck(const Deck &other);

  /// Replace this deck's contents with a copy of another deck.
  /// TODO: Free the old cards, copy the new ones, and survive d = d.
  Deck &operator=(const Deck &other);

  /// Put all the cards in random order and start dealing from the top again.
  /// @param gen the random number generator to draw from
  void shuffle(std::mt19937 &gen);

  /// Take the top card.
  /// @pre the deck is not empty
  /// @return the card that was on top
  Card deal();

  /// @return true if every card has been dealt
  bool empty();

  /// @return how many cards are left to deal
  int remaining();

private:
  Card *m_cards;
  int m_size;
  int m_next;
};

#endif // ARCADE_DECK_H
