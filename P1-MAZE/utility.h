// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725
// A file meant to store useful functions and structs
#pragma once
#include <vector>
#include <string>
using namespace std;


bool between(size_t a, size_t b, size_t c, size_t d);

size_t char_to_int(char whatever);

char int_to_char(size_t whatever);

bool valid_space(char space, char compared);

size_t color_stuff(const vector<vector<vector<char>>> &backtracking, size_t row, size_t col, size_t colors);

struct Three_Colors
{
	Three_Colors() : color(' '), row(0), col(0) {}
	Three_Colors(char color, size_t row, size_t column) : color(color), row(row), col(column) {}


	char color;
	size_t row;
	size_t col;
};