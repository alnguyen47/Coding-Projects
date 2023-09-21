// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <getopt.h>
#include "puzzle.h"
using namespace std;


int main(int argc, char *argv[])
{
	ios_base::sync_with_stdio(false);
	
	Puzzle pz;

	pz.read_command_line(argc, argv);

	pz.read_input();

	pz.find_solution();

	return 0;
}