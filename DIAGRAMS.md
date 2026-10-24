# Lab 10 diagrams

## Memory diagram: Deck after a shallow copy

A shallow copy copies the pointer, so both decks point at the same array.

```
 stack                                    heap
 +-----------------------+
 | original : Deck       |
 |   m_cards  ---------------+
 |   m_size = 52         |   |            +--------+--------+--------+-----+---------+
 |   m_next = 0          |   +----------> | [0] AC | [1] 2C | [2] 3C | ... | [51] KS |
 +-----------------------+   |            +--------+--------+--------+-----+---------+
 | copy : Deck           |   |
 |   m_cards  ---------------+
 |   m_size = 52         |
 |   m_next = 0          |
 +-----------------------+
```

Shuffling `copy` reorders the one shared array, so `original` changes too.
When both decks go out of scope each destructor does `delete[]` on the same
array, which is a double delete.

## Memory diagram: Deck after a deep copy

The copy constructor does its own `new Card[52]` and copies every card.

```
 stack                                    heap
 +-----------------------+                +--------+--------+--------+-----+---------+
 | original : Deck       |                | [0] AC | [1] 2C | [2] 3C | ... | [51] KS |
 |   m_cards  ----------------------------> +--------+--------+--------+-----+---------+
 |   m_size = 52         |
 |   m_next = 0          |
 +-----------------------+                +--------+--------+--------+-----+---------+
 | copy : Deck           |                | [0] AC | [1] 2C | [2] 3C | ... | [51] KS |
 |   m_cards  ----------------------------> +--------+--------+--------+-----+---------+
 |   m_size = 52         |
 |   m_next = 0          |
 +-----------------------+
```

Each deck owns its own array. Shuffling `copy` leaves `original` alone and
each destructor frees only its own array.

## Flowchart: shuffle(gen)

```
        ( start )
            |
            v
   [ i = m_size - 1 ]
            |
            v
      < i > 0 ? > ---- no ----> [ m_next = 0 ] ---> ( return )
            |
           yes
            |
            v
   [ j = random in 0..i ]
            |
            v
   [ swap m_cards[i], m_cards[j] ]
            |
            v
       [ i = i - 1 ]
            |
            +----------> back to < i > 0 ? >
```

## Flowchart: copy constructor  Deck(const Deck &other)

```
        ( start )
            |
            v
   [ m_cards = new Card[other.m_size] ]
   [ m_size  = other.m_size           ]
   [ m_next  = other.m_next           ]
            |
            v
        [ i = 0 ]
            |
            v
    < i < m_size ? > ---- no ----> ( done )
            |
           yes
            |
            v
   [ m_cards[i] = other.m_cards[i] ]
            |
            v
       [ i = i + 1 ]
            |
            +----------> back to < i < m_size ? >
```

## Flowchart: operator=(const Deck &other)

```
        ( start )
            |
            v
   < this == &other ? > ---- yes ----> ( return *this )
            |
            no
            |
            v
   [ delete[] m_cards ]
            |
            v
   [ m_cards = new Card[other.m_size] ]
   [ m_size  = other.m_size           ]
   [ m_next  = other.m_next           ]
            |
            v
        [ i = 0 ]
            |
            v
    < i < m_size ? > ---- no ----> ( return *this )
            |
           yes
            |
            v
   [ m_cards[i] = other.m_cards[i] ]
            |
            v
       [ i = i + 1 ]
            |
            +----------> back to < i < m_size ? >
```
