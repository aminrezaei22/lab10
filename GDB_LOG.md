# GDB log

One dated section per week. Paste the commands you used and the output you saw, then explain what it means in your own words. Pasted output with no explanation earns partial credit at most.

## Week 10 (Lab 5)

### Part A: basics with `buggy_rooms.cpp`

Build and run:

```
cmake -S . -B build
cmake --build build --target buggy_rooms
./build/buggy_rooms
```

Record:

- The backtrace (`bt`) after the crash:

```
(gdb) bt
#0  0x00007ffff7d6bd44 in std::operator<< (std::ostream&, std::string const&) () from libstdc++.so.6
#1  0x00005555555563de in describe (room=0x0) at gdb/buggy_rooms.cpp:36
#2  0x00005555555566b6 in main () at gdb/buggy_rooms.cpp:53
```

  The crash is inside the library, but it was called from my `describe` with `room=0x0`.

- The function and line number where your own code crashed (use `frame N` to move to it):

```
(gdb) frame 1
#1  0x00005555555563de in describe (room=0x0) at gdb/buggy_rooms.cpp:36
```

  `describe`, line 36, reading `room->name` through a null pointer.

- The value of the pointer that caused the crash (`print`):

```
(gdb) print room
$1 = (const Room *) 0x0
```

  The pantry has no east exit, so the third `move` east returned `nullptr`.

- How you stepped through the program (`break main`, `run`, `next`, `step`): list the commands and what each showed:

```
(gdb) break main          stops at line 39
(gdb) run
(gdb) next  (x6)          runs make_room and connect one line at a time
(gdb) print pantry->exits
$3 = {0x0, 0x0, 0x0, 0x0}      the pantry has no exits
(gdb) break move(Room*, Direction)
(gdb) finish
Value returned is $1 = (Room *) 0x55555556d370   (kitchen)
(gdb) continue
(gdb) finish
Value returned is $2 = (Room *) 0x55555556d410   (pantry)
(gdb) continue
(gdb) finish
Value returned is $3 = (Room *) 0x0              (third move east)
```

- The fix, and how you checked it:

  In `main` I check the result of `move` and print "There is no exit that way." instead of describing a null room. Rebuilt and ran it: no crash, exit code 0.

### Part B: memory with `deck_bugs.cpp`

Build with AddressSanitizer and run each bug on its own (`./deck_bugs 1`, `2`, `3`).

```
./build-asan/deck_bugs 1
==560660==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 416 byte(s) in 1 object(s) allocated from:
    #1 in deal_a_hand() gdb/deck_bugs.cpp:40

./build-asan/deck_bugs 2
==560664==ERROR: AddressSanitizer: attempting double-free on 0x514000000040
    #1 in RawDeck::~RawDeck()
    #2 in copy_a_deck() gdb/deck_bugs.cpp:50
freed by thread T0 here: RawDeck::~RawDeck() from copy_a_deck() gdb/deck_bugs.cpp:50
previously allocated by thread T0 here: RawDeck::RawDeck() from copy_a_deck() gdb/deck_bugs.cpp:47

./build-asan/deck_bugs 3
==560667==ERROR: AddressSanitizer: attempting double-free on 0x504000000010
    #1 in clear_a_table() gdb/deck_bugs.cpp:59
freed by thread T0 here: clear_a_table() gdb/deck_bugs.cpp:57
previously allocated by thread T0 here: clear_a_table() gdb/deck_bugs.cpp:54
```

For each bug, write one or two sentences: what kind of error AddressSanitizer reported, where the memory was allocated, and where it was freed.

| Bug | Report kind | Allocated at | Freed at | Cause in one sentence |
| --- | --- | --- | --- | --- |
| 1 | memory leak | `deal_a_hand()` line 40 | never | The hand is allocated with `new[]` and never deleted. |
| 2 | double-free | `RawDeck()` from line 47 | `~RawDeck()` twice at line 50 | No copy constructor, so `backup` shares the pointer with `original` and both destructors delete it. |
| 3 | double-free | line 54 | line 57 and line 59 | `delete[] table` runs twice on the same pointer. |

Confirm one bug in GDB. Use at least two of `watch`, `finish`, and `x`. Paste the session and explain what each command showed:

Bug 2, with the debug build:

```
(gdb) break copy_a_deck
(gdb) run 2
(gdb) next
(gdb) next
48	  RawDeck backup = original;
(gdb) print original
$1 = {cards_ = 0x55555556f2b0, size_ = 52}
(gdb) watch backup.cards_
(gdb) continue
Hardware watchpoint 2: backup.cards_
Old value = (Card *) 0x555555559047
New value = (Card *) 0x55555556f2b0
(gdb) x/2xg &original
0x7fffffffe960:	0x000055555556f2b0	0x0000000000000034
(gdb) x/2xg &backup
0x7fffffffe970:	0x000055555556f2b0	0x00007fffffffea00
(gdb) delete 2
(gdb) finish
Backup top card: AC
free(): double free detected in tcache 2
Program received signal SIGABRT, Aborted.
```

- `watch` stopped when `backup.cards_` was written and the new value is the same pointer `original` has.
- `x` on both objects shows the first word (`cards_`) is the same heap address `0x55555556f2b0`.
- `finish` ran to the end of the function where both destructors freed that one array and glibc aborted.

Fixes for all three bugs (describe each change):

1. Added `delete[] hand;` at the end of `deal_a_hand()`.
2. Added a copy constructor and `operator=` to `RawDeck` that allocate a new array and copy the cards.
3. Removed the second `delete[] table;` in `clear_a_table()`.

Rebuilt with AddressSanitizer: `./build-asan/deck_bugs 1`, `2` and `3` all exit 0 with no report.
