#include "input.h"

#include <iostream>

bool read_line(std::string &line) {
  return static_cast<bool>(std::getline(std::cin, line));
}

bool prompt_line(const std::string &prompt, std::string &line) {
  std::cout << prompt << std::flush;
  return read_line(line);
}

void split_first(const std::string &line, std::string &first,
                 std::string &rest) {
  const std::string::size_type space{line.find(' ')};
  if (space == std::string::npos) {
    first = line;
    rest.clear();
    return;
  }

  first = line.substr(0, space);
  rest = line.substr(space + 1);
}

bool parse_int(const std::string &text, int &value) {
  if (text.empty()) {
    return false;
  }
  const bool negative{text[0] == '-'};
  const std::string::size_type start = negative ? 1 : 0;
  if (start == text.size()) {
    return false;
  }
  long long total{0};

  for (std::string::size_type i{start}; i < text.size(); ++i) {
    if (text[i] < '0' || text[i] > '9') {
      return false;
    }
    total = total * 10 + (text[i] - '0');
    if (total > 1000000000) {
      return false;
    }
  }
  value = static_cast<int>(negative ? -total : total);
  return true;
}
