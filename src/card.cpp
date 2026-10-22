#include "card.h"

Card::Card() : m_rank{1}, m_suit{Suit::spades} {}

Card::Card(int rank, Suit suit) : m_rank{rank}, m_suit{suit} {}

int Card::rank() const { return m_rank; }

Suit Card::suit() const { return m_suit; }

std::string Card::to_string() const {
  std::string text;
  switch (m_rank) {
  case 1:
    text = "A";
    break;
  case 10:
    text = "10";
    break;
  case 11:
    text = "J";
    break;
  case 12:
    text = "Q";
    break;
  case 13:
    text = "K";
    break;
  default:
    text = std::string(1, static_cast<char>('0' + m_rank));
    break;
  }
  switch (m_suit) {
  case Suit::clubs:
    text += 'C';
    break;
  case Suit::diamonds:
    text += 'D';
    break;
  case Suit::hearts:
    text += 'H';
    break;
  case Suit::spades:
    text += 'S';
    break;
  }
  return text;
}
