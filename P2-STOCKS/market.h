// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#pragma once
#include "utility.h"
#include <string>
using namespace std;

class Market {

public:
	Market() : is_verbose(false), is_median(false), is_trader(false), is_time(false), mode(" ") {}

	/////// COMMAND LINE AND INPUT ///////////////////////////////////

	// Read the command line arguments
	void read_command_line(int argc, char* argv[]);

	// Read in the input
	void read_input();
	
	// Solve our solution
	void solve_solution(istream & inputStream, unsigned int num_stocks, unsigned int num_traders);


private:
	bool is_verbose, is_median, is_trader, is_time;
	string mode;
	
	
};