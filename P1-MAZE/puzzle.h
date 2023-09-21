// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725

#include <vector>
using namespace std;
#include "utility.h"

// The class will be able to read the command line input and
// read in the file as well as solve the problem
class Puzzle
{
public:
	// Constructor for the Puzzle
	Puzzle() 
		: maze(10, vector<char>(10, ' ')) , is_queue(false), is_stack(false), is_list(false), colors(0), rows(0), cols(0)
	, loc_r_ques(-1), loc_c_ques(-1), loc_r_at(-1), loc_c_at(-1) { }

	/////// COMMAND LINE AND INPUT ///////////////////////////////////

	// Read the command line arguments
	void read_command_line(int argc, char** argv);

	// Read in the input
	void read_input();


	//////// ALGORITHM PART //////////////////////////////////////////

	// Finding the path with either a queue or a stack
	void find_solution();

	
	/////// PRINTING OUTPUT //////////////////////////////////////////

	// Print the maze
	void print_maze_list(vector<vector<vector<char>>> &backtrack);


private:
	// The vector will store integers
	vector<vector<char>> maze;
	bool is_queue, is_stack, is_list;
	size_t colors, rows, cols;
	size_t loc_r_ques, loc_c_ques, loc_r_at, loc_c_at;
};
