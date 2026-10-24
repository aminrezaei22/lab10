#ifndef ARCADE_DECK_H
#define ARCADE_DECK_H

#include <random>

#include "card.h"

/// @file deck.h
/// @brief A deck of 52 cards that owns its memory.

/// The number of cards in a full deck.
inline constexpr int deck_size = 52;

// Class invariant:
//  m_cards points to a heap array of m_size cards that this deck owns and is
//  the only owner of. m_size is deck_size (52). m_next is the index of the
//  next card to deal and is always between 0 and m_size; when m_next == m_size
//  the deck is empty.
//
// Self-assignment:
//  If operator= did not check for d = d, it would first delete[] m_cards, and
//  then try to copy from other.m_cards, which is the same pointer that was
//  just freed. The copy would read freed memory, the deck would end up holding
//  garbage, and the destructor would later delete[] the array a second time.

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
  bool empty() const;

  /// @return how many cards are left to deal
  int remaining() const;

private:
  Card *m_cards;
  int m_size;
  int m_next;
};

#endif // ARCADE_DECK_H
