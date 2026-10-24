# Lab 10 diagrams

## Deck after a shallow copy

```
 stack                                heap
 original : Deck
   m_cards ------------------+
   m_size = 52               |       +--------+--------+-----+---------+
   m_next = 0                +-----> | [0] AC | [1] 2C | ... | [51] KS |
 copy : Deck                 |       +--------+--------+-----+---------+
   m_cards ------------------+
   m_size = 52
   m_next = 0
```

Both decks point at the same array, so shuffling one changes the other and both destructors delete it.

## Deck after a deep copy

```
 stack                                heap
 original : Deck                     +--------+--------+-----+---------+
   m_cards ------------------------> | [0] AC | [1] 2C | ... | [51] KS |
   m_size = 52                       +--------+--------+-----+---------+
   m_next = 0
 copy : Deck                         +--------+--------+-----+---------+
   m_cards ------------------------> | [0] AC | [1] 2C | ... | [51] KS |
   m_size = 52                       +--------+--------+-----+---------+
   m_next = 0
```

Each deck owns its own array.

## shuffle(gen)

```
 ( start )
     |
 [ i = m_size - 1 ]
     |
 < i > 0 ? > --no--> [ m_next = 0 ] --> ( return )
     | yes
 [ j = random in 0..i ]
     |
 [ swap m_cards[i], m_cards[j] ]
     |
 [ i = i - 1 ]
     |
     +--> back to < i > 0 ? >
```

## Copy constructor Deck(const Deck &other)

```
 ( start )
     |
 [ m_cards = new Card[other.m_size]; m_size = other.m_size; m_next = other.m_next ]
     |
 [ i = 0 ]
     |
 < i < m_size ? > --no--> ( done )
     | yes
 [ m_cards[i] = other.m_cards[i] ]
     |
 [ i = i + 1 ]
     |
     +--> back to < i < m_size ? >
```

## operator=(const Deck &other)

```
 ( start )
     |
 < this == &other ? > --yes--> ( return *this )
     | no
 [ delete[] m_cards ]
     |
 [ m_cards = new Card[other.m_size]; m_size = other.m_size; m_next = other.m_next ]
     |
 [ i = 0 ]
     |
 < i < m_size ? > --no--> ( return *this )
     | yes
 [ m_cards[i] = other.m_cards[i] ]
     |
 [ i = i + 1 ]
     |
     +--> back to < i < m_size ? >
```
