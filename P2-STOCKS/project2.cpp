// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <getopt.h>
#include "market.h"
using namespace std;

int main(int argc, char *argv[])
{
	// Do command line
	// Then do input
	ios_base::sync_with_stdio(false);
	Market ma;
	ma.read_command_line(argc, argv);
	ma.read_input();

	return 0;
}