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
(gdb) run
You are in the hall. A long, cold hall.
You are in the kitchen. Pots hang from hooks.
You are in the pantry. Shelves of jars.

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7d6bd44 in std::operator<< (std::ostream&, std::string const&) () from /lib/x86_64-linux-gnu/libstdc++.so.6
(gdb) bt
#0  0x00007ffff7d6bd44 in std::operator<< (std::ostream&, std::string const&) () from /lib/x86_64-linux-gnu/libstdc++.so.6
#1  0x00005555555563de in describe (room=0x0) at gdb/buggy_rooms.cpp:36
#2  0x00005555555566b6 in main () at gdb/buggy_rooms.cpp:53
```

  Frame 0 is inside the library (printing a string). Frame 1 is my `describe` function and it was called with `room=0x0`, so the real problem is a null pointer that came from `main` at line 53.

- The function and line number where your own code crashed (use `frame N` to move to it):

```
(gdb) frame 1
#1  0x00005555555563de in describe (room=0x0) at gdb/buggy_rooms.cpp:36
36	            << "\n";
```

  `describe` at line 36, where it does `room->name`. The pointer is null so reading the name crashes.

- The value of the pointer that caused the crash (`print`):

```
(gdb) print room
$1 = (const Room *) 0x0
(gdb) frame 2
#2  0x00005555555566b6 in main () at gdb/buggy_rooms.cpp:53
53	    describe(current);
(gdb) print current
$2 = (Room *) 0x0
(gdb) print d
$3 = Direction::east
```

  `room` is `0x0`. In `main`, `current` is also `0x0` and the direction that produced it was `east`. The walk goes east three times, but there are only two east exits (hall -> kitchen -> pantry), so the third `move` returns the pantry's unset east exit, which is `nullptr`.

- How you stepped through the program (`break main`, `run`, `next`, `step`): list the commands and what each showed:

```
(gdb) break main
Breakpoint 1 at 0x2431: file gdb/buggy_rooms.cpp, line 39.
(gdb) run
Breakpoint 1, main () at gdb/buggy_rooms.cpp:39
(gdb) next
40	  Room *hall = make_room("hall", "A long, cold hall.");
(gdb) next
41	  Room *kitchen = make_room("kitchen", "Pots hang from hooks.");
(gdb) next
42	  Room *pantry = make_room("pantry", "Shelves of jars.");
(gdb) next
43	  connect(hall, Direction::east, kitchen);
(gdb) next
44	  connect(kitchen, Direction::east, pantry);
(gdb) next
46	  Room *current = hall;
(gdb) print hall->exits
$2 = {0x0, 0x55555556d370, 0x0, 0x0}
(gdb) print pantry->exits
$3 = {0x0, 0x0, 0x0, 0x0}
```

  `next` runs one line at a time without going into the functions. After the two `connect` calls, `hall->exits[1]` (east) points at the kitchen but every exit of the pantry is still `0x0`.

```
(gdb) break move(Room*, Direction)
(gdb) run
Breakpoint 1, move (current=0x55555556d2d0, d=Direction::east) at gdb/buggy_rooms.cpp:31
(gdb) finish
Value returned is $1 = (Room *) 0x55555556d370
(gdb) continue
Breakpoint 1, move (current=0x55555556d370, d=Direction::east) at gdb/buggy_rooms.cpp:31
(gdb) finish
Value returned is $2 = (Room *) 0x55555556d410
(gdb) continue
Breakpoint 1, move (current=0x55555556d410, d=Direction::east) at gdb/buggy_rooms.cpp:31
(gdb) finish
Value returned is $3 = (Room *) 0x0
(gdb) next
53	    describe(current);
(gdb) print current
$5 = (Room *) 0x0
```

  `finish` runs until the current function returns and shows the return value. The first two `move` calls return real rooms, the third returns `0x0`, and that null is stored in `current` and passed to `describe`.

- The fix, and how you checked it:

  In the loop in `main` I store the result of `move` in a `next` pointer first. If it is `nullptr` I print "There is no exit that way." and keep `current` where it was, otherwise I move and describe the room. Rebuilt with `cmake --build build --target buggy_rooms` and ran it:

```
You are in the hall. A long, cold hall.
You are in the kitchen. Pots hang from hooks.
You are in the pantry. Shelves of jars.
There is no exit that way.
There is no exit that way.
```

  No crash and the exit code is 0. (The last step is west from the pantry; the pantry only has an east link going in, nothing going out, so that is also "no exit".)

### Part B: memory with `deck_bugs.cpp`

Build with AddressSanitizer and run each bug on its own (`./deck_bugs 1`, `2`, `3`).

```
cmake -S . -B build-asan -DENABLE_ASAN=ON
cmake --build build-asan --target deck_bugs
./build-asan/deck_bugs 1
./build-asan/deck_bugs 2
./build-asan/deck_bugs 3
```

Bug 1:

```
==560660==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 416 byte(s) in 1 object(s) allocated from:
    #0 0x7f43d3cfe6c8 in operator new[](unsigned long)
    #1 0x58ea60c95651 in deal_a_hand() gdb/deck_bugs.cpp:40
SUMMARY: AddressSanitizer: 416 byte(s) leaked in 1 allocation(s).
```

Bug 2:

```
==560664==ERROR: AddressSanitizer: attempting double-free on 0x514000000040 in thread T0:
    #0 0x77d8cd8ff1f8 in operator delete[](void*)
    #1 0x5e9c27ba5963 in RawDeck::~RawDeck()
    #2 0x5e9c27ba4b25 in copy_a_deck() gdb/deck_bugs.cpp:50
0x514000000040 is located 0 bytes inside of 416-byte region
freed by thread T0 here:
    #1 0x5e9c27ba5963 in RawDeck::~RawDeck()
    #2 0x5e9c27ba4b16 in copy_a_deck() gdb/deck_bugs.cpp:50
previously allocated by thread T0 here:
    #1 0x5e9c27ba5621 in RawDeck::RawDeck()
    #2 0x5e9c27ba49c8 in copy_a_deck() gdb/deck_bugs.cpp:47
SUMMARY: AddressSanitizer: double-free in operator delete[](void*)
```

Bug 3:

```
==560667==ERROR: AddressSanitizer: attempting double-free on 0x504000000010 in thread T0:
    #0 0x73513f4ff1f8 in operator delete[](void*)
    #1 0x570180af3f00 in clear_a_table() gdb/deck_bugs.cpp:59
0x504000000010 is located 0 bytes inside of 40-byte region
freed by thread T0 here:
    #1 0x570180af3eca in clear_a_table() gdb/deck_bugs.cpp:57
previously allocated by thread T0 here:
    #1 0x570180af3cc2 in clear_a_table() gdb/deck_bugs.cpp:54
SUMMARY: AddressSanitizer: double-free in operator delete[](void*)
```

For each bug, write one or two sentences: what kind of error AddressSanitizer reported, where the memory was allocated, and where it was freed.

| Bug | Report kind | Allocated at | Freed at | Cause in one sentence |
| --- | --- | --- | --- | --- |
| 1 | LeakSanitizer: detected memory leaks (416 bytes = 52 cards) | `deal_a_hand()` line 40, `new Card[52]` | never | The hand is allocated with `new[]` but the function returns without `delete[]`, so the 52 cards are leaked. |
| 2 | AddressSanitizer: double-free | `RawDeck::RawDeck()` called from `copy_a_deck()` line 47 | twice in `RawDeck::~RawDeck()` at the end of `copy_a_deck()` line 50 | `RawDeck` has no copy constructor, so `backup = original` copies the `cards_` pointer and both destructors `delete[]` the same array. |
| 3 | AddressSanitizer: double-free | `clear_a_table()` line 54, `new Card[5]` | line 57 and again line 59 | `table` is deleted once, then `delete[] table` is run a second time on the same pointer. |

Confirm one bug in GDB. Use at least two of `watch`, `finish`, and `x`. Paste the session and explain what each command showed:

I confirmed bug 2 (the shallow copy) with the normal debug build of `deck_bugs`.

```
(gdb) break copy_a_deck
Breakpoint 1 at 0x24f8: file gdb/deck_bugs.cpp, line 46.
(gdb) run 2
Breakpoint 1, copy_a_deck () at gdb/deck_bugs.cpp:46
(gdb) next
47	  RawDeck original;
(gdb) next
48	  RawDeck backup = original;
(gdb) print original
$1 = {cards_ = 0x55555556f2b0, size_ = 52}
(gdb) watch backup.cards_
Hardware watchpoint 2: backup.cards_
(gdb) continue
Hardware watchpoint 2: backup.cards_

Old value = (Card *) 0x555555559047
New value = (Card *) 0x55555556f2b0
0x000055555555651f in copy_a_deck () at gdb/deck_bugs.cpp:48
48	  RawDeck backup = original;
(gdb) print original.cards_ == backup.cards_
$2 = true
(gdb) x/2xg &original
0x7fffffffe960:	0x000055555556f2b0	0x0000000000000034
(gdb) x/2xg &backup
0x7fffffffe970:	0x000055555556f2b0	0x00007fffffffea00
(gdb) x/4dw original.cards_
0x55555556f2b0:	1	0	2	0
(gdb) set var backup.cards_[0].m_rank = 13
(gdb) print original.cards_[0].to_string()
$3 = "KC"
(gdb) delete 2
(gdb) finish
Run till exit from #0  copy_a_deck () at gdb/deck_bugs.cpp:48
Backup top card: KC
free(): double free detected in tcache 2

Program received signal SIGABRT, Aborted.
```

- `watch backup.cards_` stopped the program the moment `backup.cards_` was written. The new value `0x55555556f2b0` is exactly the pointer that `original.cards_` already had, so the "copy" only copied the pointer.
- `x/2xg &original` and `x/2xg &backup` dump the two objects as raw 8-byte words. The first word of both is `0x55555556f2b0`, the same heap address. (The second word is `size_` plus padding: `0x34` is 52.)
- `x/4dw original.cards_` shows the start of the shared array as ints: rank 1 suit 0, rank 2 suit 0, which is AC then 2C.
- Changing a card through `backup` and then printing it through `original` gives `"KC"`, which proves the two decks share one array.
- `finish` ran to the end of `copy_a_deck`. Both destructors ran, each did `delete[]` on the same pointer, and glibc aborted with `free(): double free detected`.

Fixes for all three bugs (describe each change):

1. Bug 1 (leak): added `delete[] hand;` at the end of `deal_a_hand()` so the array the function allocated is released before it returns.
2. Bug 2 (shallow copy): gave `RawDeck` a copy constructor and an `operator=` that allocate a new `Card[size_]` and copy the cards one by one, with the `this == &other` check in `operator=`. Now `backup` owns its own array and each destructor frees its own memory.
3. Bug 3 (double delete): removed the second `delete[] table;` at the end of `clear_a_table()` so the array is freed exactly once.

Checked by rebuilding with AddressSanitizer and running `./build-asan/deck_bugs 1`, `2` and `3` again. All three print their line and exit with 0 and no report.
