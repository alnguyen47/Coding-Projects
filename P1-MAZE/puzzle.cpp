// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <getopt.h>
#include "puzzle.h"
using namespace std;

// Read the command line arguments
void Puzzle::read_command_line(int argc, char** argv)
{
	int option_index = 0, option = 0;
	int counter_qu = 0;
	int counter_st = 0;

	// Don't display getopt error messages about options
	opterr = false;

	// Use getopt to find command line options
	struct option longOpts[] = {
		{ "queue", no_argument, nullptr, 'q' },
		{ "stack", no_argument, nullptr, 's' },
		{ "output", required_argument, nullptr, 'o' },
		{ "help", no_argument, nullptr, 'h' },
		{ nullptr, 0, nullptr, '\0' } };

	// Now determine the choices for each option
	while ((option = getopt_long(argc, argv, "o:qsh", longOpts, &option_index)) != -1) {

		switch (option) {
		case 'q':
			counter_qu += 1;
			if (counter_qu != 1 || counter_st > 0)
			{
				cerr << ("No data was read in! Refer to the help option to see program usage.");
				exit(1);
			}
			is_queue = true;
			break;

		case 's':
			counter_st += 1;
			if (counter_st != 1 || counter_qu > 0)
			{
				cerr << ("No data was read in! Refer to the help option to see program usage.");
				exit(1);
			}
			is_queue = false;
			break;

		case 'o':
			if (string(optarg) == "list" || string(optarg) == "l")
			{
				// Call a function that outputs result as list
				is_list = true;
			}
			else if (string(optarg) == "map" || string(optarg) == "m")
			{
				// Call a function that outputs result as map
				is_list = false;
			}
			else
			{
				// return error
				cerr << ("No data was read in! Refer to the help option to see program usage.");
				exit(1);
			}
			break;

		case 'h':
			cout << "This program reads a CSV file that has a map\n"
				<< "of characters. \n"
				<< endl;
			exit(0);

		default:
			cerr << ("No data was read in! Refer to the help option to see program usage.");
			exit(1);
		}

	}

	// Check for if stack and queue are 0
	if (counter_qu == 0 && counter_st == 0)
	{
		cerr << ("No data was read in! Refer to the help option to see program usage.");
		exit(1);
	}

}

// Read from the file given
void Puzzle::read_input()
{
	string temp;
	int counter_change1 = 0;
	int counter_change2 = 0;

	// First take the numbers in
	cin >> colors >> rows >> cols;

	// Error checking
	if (rows == 0 || cols == 0 || colors > 26) //|| rows > 200000 || cols > 200000 || (colors * rows * cols >= 400000000))
	{
		cerr << ("No data was read in! Refer to the help option to see program usage.");
		exit(1);
	}

	// Deals with the remaining whitespace
	getline(cin, temp);

	// Resize the 3-D vector in order to match the values
	maze.resize(rows);
	size_t row = 0;
	size_t col = 0;

	// Now what could be next are comments
	while (getline(cin, temp))
	{
		// If it does not begin with two slashes then read the map
		if (temp[0] != '/')
		{
			maze[row].resize(cols);
			// The lines of the map will be passed as an entire string
			for (size_t i = 0; i < temp.size(); i++)
			{
				maze[row][col] = temp[i];
				if (maze[row][col] == '@')
				{
					loc_r_at = row;
					loc_c_at = col;
					counter_change1++;
				}
				if (maze[row][col] == '?')
				{
					loc_r_ques = row;
					loc_c_ques = col;
					counter_change2++;
				}

				// ERROR CHECK
				if (!isalpha(maze[row][col]) && maze[row][col] != '#' && maze[row][col] != '?' && maze[row][col] != '@'
					&& maze[row][col] != '^' && maze[row][col] != '.')
				{
					cerr << ("No data was read in! Refer to the help option to see program usage.");
					exit(1);
				}
				if (isalpha(maze[row][col]) && (tolower(maze[row][col]) > int_to_char(colors)))
				{
					cerr << ("No data was read in! Refer to the help option to see program usage.");
					exit(1);
				}
				col += 1;
			}
			col = 0;
			row += 1;
		}
	}

	// Error Check for @ and ?
	if (counter_change1 != 1 || counter_change2 != 1)
	{
		cerr << ("No data was read in! Refer to the help option to see program usage.");
		exit(1);
	}
}

/////// ALGORITHM STUFF /////////////////////////////

void Puzzle::find_solution()
{
	// 1. Read the map and get the location of @ (could also do a struct in order to make it easier)
	deque<Three_Colors> reachable;

	// 3D vector to hold our directions
	// It should hold color state and then also the directions. So you determine if it has been reached if there was a location assigned in it
	vector<vector<vector<char>>> backtracking(colors + 1, vector<vector<char>>(rows, vector<char>(cols, ' ')));

	Three_Colors temp('^', loc_r_at, loc_c_at);
	Three_Colors temp1(' ', 0, 0);
	// Find the location of @, first item in deque
	reachable.push_back(temp);

	// Assign the direction
	// 0 = ^, 1 = a, ..., 26 = z
	backtracking[0][loc_r_at][loc_c_at] = '*';

	// 2. Now we loop the deque while it is not empty
	while (!reachable.empty())
	{
		// 3. Now we look at the first element in the queue if it is a queue
		if (is_queue)
		{
			temp.color = reachable.front().color;
			temp.row = reachable.front().row;
			temp.col = reachable.front().col;
			reachable.pop_front();
		}
		else
		{
			temp.color = reachable.back().color;
			temp.row = reachable.back().row;
			temp.col = reachable.back().col;
			reachable.pop_back();
		}

		// Check if the color state has been reached, if it has then we don't want to add it
			// I went to b from a but then it went back to a as b
		if (backtracking[char_to_int(maze[temp.row][temp.col])][temp.row][temp.col] == ' ' && ((islower(maze[temp.row][temp.col]) && temp.color == '^')
			|| (islower(temp.color) && maze[temp.row][temp.col] == '^') || (islower(temp.color) && islower(maze[temp.row][temp.col])
				&& temp.color != maze[temp.row][temp.col])))
		{
			// If its an 'a', then it will check the index to be 1
			// Need to set this equal to the last color it was before stepping onto the new button
			backtracking[char_to_int(maze[temp.row][temp.col])][temp.row][temp.col] = temp.color;
			temp1.color = maze[temp.row][temp.col];
			temp1.row = temp.row;
			temp1.col = temp.col;
			reachable.push_back(temp1);
		}
		// If it is not a button, add the north, east, south, west locations
		else if ((!islower(maze[temp.row][temp.col]) && maze[temp.row][temp.col] != '^') || maze[temp.row][temp.col] == temp.color)
		{
			// North
			if (between(temp.row - 1, rows, temp.col, cols))
			{
				// Check if its a valid space
				if (backtracking[char_to_int(temp.color)][temp.row - 1][temp.col] == ' ' && valid_space(maze[temp.row - 1][temp.col], temp.color))
				{
					// backtracking should say that we came from the south (south = 1)
					// We need to look at the complete state
					backtracking[char_to_int(temp.color)][temp.row - 1][temp.col] = '2';

					// Determine if you are on the question mark
					if (maze[temp.row - 1][temp.col] == '?') { break; }

					temp1.color = temp.color;
					temp1.row = temp.row - 1;
					temp1.col = temp.col;
					reachable.push_back(temp1);
				}
			}
			// East
			if (between(temp.row, rows, temp.col + 1, cols))
			{
				if (backtracking[char_to_int(temp.color)][temp.row][temp.col + 1] == ' ' && valid_space(maze[temp.row][temp.col + 1], temp.color))
				{
					// backtracking should say that we came from the west(west = 2)
					backtracking[char_to_int(temp.color)][temp.row][temp.col + 1] = '4';

					// Determine if you are on the question mark
					if (maze[temp.row][temp.col + 1] == '?') { break; }

					temp1.color = temp.color;
					temp1.row = temp.row;
					temp1.col = temp.col + 1;
					reachable.push_back(temp1);
				}
			}
			// South
			if (between(temp.row + 1, rows, temp.col, cols))
			{
				if (backtracking[char_to_int(temp.color)][temp.row + 1][temp.col] == ' ' && valid_space(maze[temp.row + 1][temp.col], temp.color))
				{
					// backtracking should say that we came from the north (north = 3)
					backtracking[char_to_int(temp.color)][temp.row + 1][temp.col] = '8';

					// Determine if you are on the question mark
					if (maze[temp.row + 1][temp.col] == '?') { break; }

					temp1.color = temp.color;
					temp1.row = temp.row + 1;
					temp1.col = temp.col;
					reachable.push_back(temp1);
				}
			}
			// West
			if (between(temp.row, rows, temp.col - 1, cols))
			{
				if (backtracking[char_to_int(temp.color)][temp.row][temp.col - 1] == ' ' && valid_space(maze[temp.row][temp.col - 1], temp.color))
				{
					// backtracking should say that we came from the east (east = 4)
					backtracking[char_to_int(temp.color)][temp.row][temp.col - 1] = '6';

					// Determine if you are on the question mark
					if (maze[temp.row][temp.col - 1] == '?') { break; }

					temp1.color = temp.color;
					temp1.row = temp.row;
					temp1.col = temp.col - 1;
					reachable.push_back(temp1);
				}
			}
		}
	}
	// Backtracking part
	print_maze_list(backtracking);
}

// If else to determine which one to pop 

////// PRINT STUFF //////////////////////////////////
void Puzzle::print_maze_list(vector<vector<vector<char>>> &backtracking)
{
	// 3D vector to store the state
	vector<Three_Colors> locs;
	bool pls_exit = true;
	int counter = 1;
	size_t temporary = color_stuff(backtracking, loc_r_ques, loc_c_ques, colors);

	// If the location where the question mark is not present, no solution(denoted by 100)
	if (temporary == 100)
	{
		// No solution
		cout << "No solution." << '\n';
		cout << "Reachable:" << '\n';
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				if (color_stuff(backtracking, i, j, colors) == 100)
				{
					maze[i][j] = '#';
				}
				cout << maze[i][j];
			}
			cout << '\n';
		}
		return;

	}
	// That means there was a location assigned to it
	// This is the current color ('b')
	size_t color_ques = temporary; // Meant to keep track of color
	size_t temp = 0; // temp variable
	Three_Colors tempo(int_to_char(color_ques), loc_r_ques, loc_c_ques);
	// First add the location of the question mark
	locs.push_back(tempo);

	// While the locations are still not = @
	while (pls_exit)
	{
		//if (color_ques == 2 && loc_r_ques == 1 && loc_c_ques == 5)
		//{
		//	cout << 'hi';
		//}
		// Checking the locations first
		if (backtracking[color_ques][loc_r_ques][loc_c_ques] == '8')
		{
			// If we have a '.', then we want to change it into a +, as well as any trap/door that is same color
			if (maze[loc_r_ques][loc_c_ques] == '.' || (isupper(maze[loc_r_ques][loc_c_ques]) && tolower(maze[loc_r_ques][loc_c_ques]) == int_to_char(color_ques))
				|| maze[loc_r_ques][loc_c_ques] == int_to_char(color_ques) || (backtracking[0][loc_r_ques][loc_c_ques] == '*' && color_ques != 0))
			{
				temp = loc_r_ques;
				backtracking[color_ques][temp][loc_c_ques] = '+';
			}
			else if ((islower(maze[loc_r_ques][loc_c_ques]) || maze[loc_r_ques][loc_c_ques] == '^') && int_to_char(color_ques) != maze[loc_r_ques][loc_c_ques])
			{
				// Probably want to update backtracking 
				temp = loc_r_ques;
				backtracking[color_ques][temp][loc_c_ques] = '%';
			}
			loc_r_ques -= 1;
		}
		else if (backtracking[color_ques][loc_r_ques][loc_c_ques] == '2')
		{
			if (maze[loc_r_ques][loc_c_ques] == '.' || (isupper(maze[loc_r_ques][loc_c_ques]) && tolower(maze[loc_r_ques][loc_c_ques]) == int_to_char(color_ques))
				|| maze[loc_r_ques][loc_c_ques] == int_to_char(color_ques) || (backtracking[0][loc_r_ques][loc_c_ques] == '*' && color_ques != 0))
			{
				temp = loc_r_ques;
				backtracking[color_ques][temp][loc_c_ques] = '+';
			}
			else if ((islower(maze[loc_r_ques][loc_c_ques]) || maze[loc_r_ques][loc_c_ques] == '^') && int_to_char(color_ques) != maze[loc_r_ques][loc_c_ques])
			{
				// Probably want to update backtracking 
				temp = loc_r_ques;
				backtracking[color_ques][temp][loc_c_ques] = '%';
			}
			loc_r_ques += 1;
			
		}
		else if (backtracking[color_ques][loc_r_ques][loc_c_ques] == '4')
		{
			if (maze[loc_r_ques][loc_c_ques] == '.' || (isupper(maze[loc_r_ques][loc_c_ques]) && tolower(maze[loc_r_ques][loc_c_ques]) == int_to_char(color_ques))
				|| maze[loc_r_ques][loc_c_ques] == int_to_char(color_ques) || (backtracking[0][loc_r_ques][loc_c_ques] == '*' && color_ques != 0))
			{ 
				temp = loc_c_ques;
				backtracking[color_ques][loc_r_ques][temp] = '+';
			}
			else if ((islower(maze[loc_r_ques][loc_c_ques]) || maze[loc_r_ques][loc_c_ques] == '^') && int_to_char(color_ques) != maze[loc_r_ques][loc_c_ques])
			{
				// Probably want to update backtracking 
				temp = loc_c_ques;
				backtracking[color_ques][loc_r_ques][temp] = '%';
			}
			loc_c_ques -= 1;
		}
		else if (backtracking[color_ques][loc_r_ques][loc_c_ques] == '6')
		{
			if (maze[loc_r_ques][loc_c_ques] == '.' || (isupper(maze[loc_r_ques][loc_c_ques]) && tolower(maze[loc_r_ques][loc_c_ques]) == int_to_char(color_ques))
				|| maze[loc_r_ques][loc_c_ques] == int_to_char(color_ques) || (backtracking[0][loc_r_ques][loc_c_ques] == '*' && color_ques != 0))
			{
				temp = loc_c_ques;
				backtracking[color_ques][loc_r_ques][temp] = '+';
			}
			else if ((islower(maze[loc_r_ques][loc_c_ques]) || maze[loc_r_ques][loc_c_ques] == '^') && int_to_char(color_ques) != maze[loc_r_ques][loc_c_ques])
			{
				// Probably want to update backtracking 
				temp = loc_c_ques;
				backtracking[color_ques][loc_r_ques][temp] = '%';
			}
			loc_c_ques += 1;
		}

		// Determine what direction it came from
		tempo.color = int_to_char(color_ques);
		tempo.row = loc_r_ques;
		tempo.col = loc_c_ques;

		locs.push_back(tempo);
		counter += 1;

		// Need to check if we are on the button of the same color, trap
		// If this is the case then the next move will likely be a different
		// color than the button we stepped on
		// Basically look for the same spot that has a different color
		// Change the symbols
		// For map update backtracking to have the correct symbols(so update with the + and all that
		if ((islower(maze[loc_r_ques][loc_c_ques]) || maze[loc_r_ques][loc_c_ques] == '^')
			&& backtracking[char_to_int(maze[loc_r_ques][loc_c_ques])][loc_r_ques][loc_c_ques] != '2'
			&& backtracking[char_to_int(maze[loc_r_ques][loc_c_ques])][loc_r_ques][loc_c_ques] != '4'
			&& backtracking[char_to_int(maze[loc_r_ques][loc_c_ques])][loc_r_ques][loc_c_ques] != '6'
			&& backtracking[char_to_int(maze[loc_r_ques][loc_c_ques])][loc_r_ques][loc_c_ques] != '8'
			&& color_ques != char_to_int(backtracking[color_ques][loc_r_ques][loc_c_ques]))
		{
			
			temp = color_ques;
			// Change the color to the new color
			color_ques = char_to_int(backtracking[color_ques][loc_r_ques][loc_c_ques]);

			// Now update the current square to be @
			backtracking[temp][loc_r_ques][loc_c_ques] = '@';
			tempo.color = int_to_char(color_ques);
			tempo.row = loc_r_ques;
			tempo.col = loc_c_ques;
			locs.push_back(tempo);
			counter += 1;
		}

		// If we reach the starting point, then we want to leave the loop 
		if (backtracking[color_ques][loc_r_ques][loc_c_ques] == '*')
		{
			break;
		}
	}

	// Resize my vector
	locs.resize(counter);
	temporary = locs.size();

	// Now we have the full path from @ -> ?
	/////////////////// List output //////////////////////////////////
	if (is_list)
	{
		do
		{
			temporary -= 1;
			cout << "(" << locs[temporary].color << ", " << "(" << locs[temporary].row << ", " << locs[temporary].col << "))" << '\n';
		} while (temporary > 0);
		return;
	}
	////////////////// Map output ////////////////////////////////////
	else
	{
		for (size_t i = 0; i < colors + 1; i++)
		{
			//if (i == 1)
			//{
			//	cout << "hi";
			//}
			cout << "// color " << int_to_char(i) << '\n';
	
			// Now we have a vector containing all of those elements
			for (size_t j = 0; j < rows; j++)
			{
				for (size_t k = 0; k < cols; k++)
				{
					// What we can do is look first at the backtracking
					// The only squares we want to print out from that backtracking
					// are those with symbols @, + or %
					if (backtracking[i][j][k] == '+' || backtracking[i][j][k] == '%' || backtracking[i][j][k] == '@')
					{
						cout << backtracking[i][j][k];
					}
					// Either we are at the starting point and the color is different or we are at a door and it is equal to the current color we are on
					// Or there is a button that is not on the path but is of the same color as the current color
					else if ((backtracking[0][j][k] == '*' && i != 0) || (isupper(maze[j][k]) && tolower(maze[j][k]) == int_to_char(i))
						|| (islower(maze[j][k]) && maze[j][k] == int_to_char(i)))
					{
						cout << '.';
					}
					else if (backtracking[i][j][k] != '+' || backtracking[i][j][k] != '%' || backtracking[i][j][k] != '@')
					{
						if (maze[j][k] == '^' && i == 0)
						{
							cout << '.';
						}
						else
						{
							cout << maze[j][k];
						}
					}
						
				}
				cout << '\n';
			}
		}
	}

}


