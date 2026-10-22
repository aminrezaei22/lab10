// Lab 10 checks for Card and Deck. They fail until Deck is finished.

#include <gtest/gtest.h>

#include <random>

#include "card.h"
#include "deck.h"

namespace {

// Deal a whole deck and count how many times each of the 52 cards appears.
// Returns true if every card appeared exactly once.
bool deals_each_card_once(Deck &deck) {
  int seen[4][14] = {};
  for (int i = 0; i < deck_size; ++i) {
    if (deck.empty()) {
      return false;
    }
    const Card card = deck.deal();
    ++seen[static_cast<int>(card.suit())][card.rank()];
  }
  for (int s = 0; s < 4; ++s) {
    for (int r = 1; r <= 13; ++r) {
      if (seen[s][r] != 1) {
        return false;
      }
    }
  }
  return deck.empty();
}

} // namespace

TEST(CardTest, ToStringUsesShortNames) {
  EXPECT_EQ(Card().to_string(), "AS");
  EXPECT_EQ((Card{10, Suit::hearts}).to_string(), "10H");
  EXPECT_EQ((Card{13, Suit::diamonds}).to_string(), "KD");
  EXPECT_EQ((Card{7, Suit::clubs}).to_string(), "7C");
}

TEST(DeckTest, NewDeckHasFiftyTwoCards) {
  Deck deck;
  EXPECT_EQ(deck.remaining(), 52);
  EXPECT_FALSE(deck.empty());
}

TEST(DeckTest, DealsEveryCardExactlyOnce) {
  Deck deck;
  EXPECT_TRUE(deals_each_card_once(deck));
}

TEST(DeckTest, ShufflePreservesTheCards) {
  Deck deck;
  std::mt19937 gen{42};
  deck.shuffle(gen);
  EXPECT_EQ(deck.remaining(), 52);
  EXPECT_TRUE(deals_each_card_once(deck));
}

TEST(DeckTest, CopyIsIndependentOfTheOriginal) {
  Deck fresh;
  ASSERT_FALSE(fresh.empty());
  const Card expected = fresh.deal();

  Deck original;
  Deck copy = original;
  std::mt19937 gen{7};
  copy.shuffle(gen);

  // Shuffling the copy must not disturb the original's order.
  const Card first = original.deal();
  EXPECT_EQ(first.to_string(), expected.to_string());
  EXPECT_EQ(original.remaining(), 51);
  EXPECT_EQ(copy.remaining(), 52);
}

TEST(DeckTest, AssignmentCopiesAndKeepsDecksIndependent) {
  Deck a;
  Deck b;
  std::mt19937 gen{3};
  b.shuffle(gen);
  b = a;
  a.shuffle(gen);
  EXPECT_TRUE(deals_each_card_once(b));
}

TEST(DeckTest, SelfAssignmentIsSafe) {
  Deck deck;
  Deck &same = deck;
  deck = same;
  EXPECT_EQ(deck.remaining(), 52);
  EXPECT_TRUE(deals_each_card_once(deck));
}
