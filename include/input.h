#ifndef ARCADE_INPUT_H
#define ARCADE_INPUT_H

#include <string>

/// @file input.h
/// @brief Shared console input helpers.

/// Read one line from standard input.
/// @param line receives the text without the trailing newline
/// @return false if input has ended (end of file), true otherwise
bool read_line(std::string &line);

/// Print a prompt, then read one line.
/// @param prompt text printed before reading
/// @param line receives the text typed by the user
/// @return false if input has ended, true otherwise
bool prompt_line(const std::string &prompt, std::string &line);

/// Split a line at its first space.
/// @param line the text to split
/// @param first receives the text before the first space (all of @p line if
/// none)
/// @param rest receives the text after the first space (empty if none)
void split_first(const std::string &line, std::string &first,
                 std::string &rest);

/// Convert a string of decimal digits (optional leading '-') to an int.
/// @param text the text to convert
/// @param value receives the result when the function returns true
/// @return true if @p text is a valid whole number that fits in an int
bool parse_int(const std::string &text, int &value);

#endif // ARCADE_INPUT_H
