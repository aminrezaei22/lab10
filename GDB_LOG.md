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
- The function and line number where your own code crashed (use `frame N` to move to it):
- The value of the pointer that caused the crash (`print`):
- How you stepped through the program (`break main`, `run`, `next`, `step`): list the commands and what each showed:
- The fix, and how you checked it:

### Part B: memory with `deck_bugs.cpp`

Build with AddressSanitizer and run each bug on its own (`./deck_bugs 1`, `2`, `3`).

For each bug, write one or two sentences: what kind of error AddressSanitizer reported, where the memory was allocated, and where it was freed.

| Bug | Report kind | Allocated at | Freed at | Cause in one sentence |
| --- | --- | --- | --- | --- |
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |

Confirm one bug in GDB. Use at least two of `watch`, `finish`, and `x`. Paste the session and explain what each command showed:

Fixes for all three bugs (describe each change):
