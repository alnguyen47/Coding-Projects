// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725

#include "utility.h"

bool between(size_t a, size_t b, size_t c, size_t d)
{
	return a < b && c < d;
}

size_t char_to_int(char whatever)
{
	if (islower(whatever))
	{
		return static_cast<uint32_t>(whatever) - 96;
	}
	else
	{
		return 0;
	}
	
}

char int_to_char(size_t whatever)
{
	if (whatever != 0)
	{
		return static_cast<char>(whatever + 96);
	}
	else
	{
		return '^';
	}

}

bool valid_space(char space, char compared)
{
	if (space == '?' || space == '^' || space == '.' || space == '@' || islower(space) || (isupper(space) && toupper(compared) == space))
	{
		return true;
	}
	return false;
}

size_t color_stuff(const vector<vector<vector<char>>> &backtracking, size_t row, size_t col, size_t colors)
{
	for (size_t i = 0; i < colors + 1; i++)
	{
		if (backtracking[i][row][col] != ' ')
		{ 
			return i;
		}
	}
	return 100;
}

