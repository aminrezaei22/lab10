#ifndef ARCADE_CARD_H
#define ARCADE_CARD_H

#include <string>

/// @file card.h
/// @brief A playing card.

/// The four suits.
enum class Suit {
  clubs,    ///< clubs
  diamonds, ///< diamonds
  hearts,   ///< hearts
  spades,   ///< spades
};

/// One playing card: a rank from 1 (ace) to 13 (king) and a suit.
class Card {
public:
  /// Make the ace of spades.
  Card();

  /// Make a card
  /// @param rank 1 = ace, 2 to 10, 11 = jack, 12 = queen, 13 = king
  /// @param suit the suit
  Card(int rank, Suit suit);

  /// @return the rank, 1 to 13
  int rank() const;

  /// @return the suit
  Suit suit() const;

  /// @return short text such as "AS", "10H", or "KD"
  std::string to_string() const;

private:
  int m_rank;
  Suit m_suit;
};

#endif // ARCADE_CARD_H
