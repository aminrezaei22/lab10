# Arcade: Week 10 starter (S10)

A text-game arcade in C++20. This is the starting point for **Lab 10: Dynamic Memory in Classes (Blackjack I)**.

## What is already here

| Part | Files | Notes |
| --- | --- | --- |
| Menu | `src/main.cpp`, `src/input.*` | Hangman, Dungeon, Blackjack, Minesweeper, Quit |
| Lab 10 | `src/card.*`, `src/deck.*`, `src/blackjack.*`, `tests/test_deck.cpp` | `Card` is finished; four `Deck` functions are yours |
| Debugging practice | `gdb/buggy_rooms.cpp`, `gdb/deck_bugs.cpp` | Not built by default |
| Notes to fill in | `GDB_LOG.md`, `READ_ALONG_NOTES.md` | See below |

The Blackjack menu entry runs the Lab 10 demonstration. Until you finish `Deck`, it tells you the deck is empty.

## Build and run

```
cmake -S . -B build                  # Debug build, -g included
cmake --build build
./build/arcade
```

With AddressSanitizer (use a separate folder):

```
cmake -S . -B build-asan -DENABLE_ASAN=ON
cmake --build build-asan
./build-asan/arcade
```

Tests:

```
cd build && ctest --output-on-failure
```

The first run downloads GoogleTest with git, so it needs network access. The six `DeckTest` checks fail until `Deck` is finished. That is expected.

Documentation (needs `doxygen`): `cmake --build build --target docs`, then open `docs/html/index.html`.

The debugging programs:

```
cmake --build build --target buggy_rooms deck_bugs
```

## Your Lab 10 tasks

1. In `src/deck.cpp`, write `Deck()`, `~Deck()`, the copy constructor, and `operator=` (safe against `d = d`).
2. In `src/deck.h`, write the class invariant comment and the self-assignment comment where the `TODO` markers are, and mark every method that does not change the deck `const`. Change the declaration in the header and the definition in `deck.cpp` together.
3. Part A and Part B of `GDB_LOG.md` (see the file).
4. Diagrams and flowcharts as described in the lab handout.
5. Commit with Conventional Commit messages, composed in the editor.

## Rules for this project

- Build with no warnings under `-Wall -Wextra`.
- Allowed headers: `<iostream>`, `<iomanip>`, `<string>`, `<fstream>`, `<stdexcept>`, `<random>`. No standard containers or `<algorithm>`. No `using namespace std;`.
- Names: `underscore_separated` functions and variables, `Capitalized` types, private data members end in `_`, enumerators are lowercase, no ALL_CAPS constants.
