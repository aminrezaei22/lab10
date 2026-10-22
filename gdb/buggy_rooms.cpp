// buggy_rooms.cpp - a tiny map walker that crashes.
// Build:  g++ -std=c++20 -Wall -Wextra -g gdb/buggy_rooms.cpp -o buggy_rooms
// Run it, then debug it with GDB.

#include <iostream>
#include <string>

enum class Direction { north, east, south, west };

struct Room {
  std::string name;
  std::string description;
  Room *exits[4];
};

Room *make_room(const std::string &name, const std::string &description) {
  Room *room = new Room;
  room->name = name;
  room->description = description;
  for (int i = 0; i < 4; ++i) {
    room->exits[i] = nullptr;
  }
  return room;
}

void connect(Room *a, Direction d, Room *b) {
  a->exits[static_cast<int>(d)] = b;
}

Room *move(Room *current, Direction d) {
  return current->exits[static_cast<int>(d)];
}

void describe(const Room *room) {
  std::cout << "You are in the " << room->name << ". " << room->description
            << "\n";
}

int main() {
  Room *hall = make_room("hall", "A long, cold hall.");
  Room *kitchen = make_room("kitchen", "Pots hang from hooks.");
  Room *pantry = make_room("pantry", "Shelves of jars.");
  connect(hall, Direction::east, kitchen);
  connect(kitchen, Direction::east, pantry);

  Room *current = hall;
  describe(current);

  const Direction walk[] = {Direction::east, Direction::east, Direction::east,
                            Direction::west};
  for (Direction d : walk) {
    current = move(current, d);
    describe(current);
  }

  delete hall;
  delete kitchen;
  delete pantry;
  return 0;
}
