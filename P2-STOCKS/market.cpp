// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <getopt.h>
#include "market.h"
#include "P2random.h"
#include <algorithm>
#include <cstdlib>
#include <numeric>
using namespace std;


// Read the command line arguments
void Market::read_command_line(int argc, char* argv[])
{
	int option_index = 0, option = 0;

	// Don't display getopt error messages about options
	opterr = false;

	// Use getopt to find command line options
	struct option longOpts[] = {
		{ "verbose", no_argument, nullptr, 'v' },
		{ "median", no_argument, nullptr, 'm' },
		{ "trader_info", no_argument, nullptr, 'i' },
		{ "time_travelers", no_argument, nullptr, 't' },
		{ nullptr, 0, nullptr, '\0' } };

	// Now determine the choices for each option
	while ((option = getopt_long(argc, argv, "vmit", longOpts, &option_index)) != -1) {

		switch (option) {
		case 'v':
			is_verbose = true;
			break;

		case 'm':
			is_median = true;
			break;

		case 'i':
			is_trader = true;
			break;

		case 't':
			is_time = true;
			break;

		default:
			cerr << ("No data was read in! Refer to the help option to see program usage.");
			exit(1);
		}

	}
}

// Read out input
void Market::read_input()
{
	unsigned int num_traders = 0;
	unsigned int num_stocks = 0;
	
	// Take in from standard input
	string temp = " ";
	// First get the comment
	getline(cin, temp);
	cin >> temp >> mode >> temp >> num_traders >> temp >> num_stocks;
	// Buyer, seller priority queue
	// Stock struct that has underlying priority queue
	// Put the stock structs into a vector

	// If the mode is PR
	unsigned int seed = 0;
	unsigned int num_orders = 0;
	unsigned int arrival_rate = 0;

	// Create isstream
	stringstream ss;

	if (mode == "PR")
	{
		cin >> temp >> seed >> temp >> num_orders >> temp >> arrival_rate;
		P2random::PR_init(ss, seed, num_traders, num_stocks, num_orders, arrival_rate);
	}
	
	// Else the mode is TL, read from either stringstream or cin
	istream & inputStream = (mode == "PR") ? ss : cin;
	solve_solution(inputStream, num_stocks, num_traders);

}

// Solve our solution
void Market::solve_solution(istream & inputStream, unsigned int num_stocks, unsigned int num_traders)
{
	// Create a vector of stock objects
	vector<Stock> stocks(num_stocks, Stock());
	
	// Potentially have to make vector for trader info, struct
	vector<Trader_info> trader_info(num_traders, Trader_info());

	// Keep track of current_time
	int current_time = 0;

	// Create temporary Buy and Sell objects
	Buyer buytemp;
	Seller selltemp;

	// Temporary variables to store input
	int timestamp = 0;
	string sell_buy = " ";
	unsigned int trader = 0;
	unsigned int stock = 0;
	int price = 0;
	int quantity = 0;
	char temp = ' ';

	unsigned int counter = 0;
	
	// Keep track of the order of orders
	unsigned int index = 0;

	int tempoo = 0;

	// 1. Print program startup output
	cout << "Processing orders..." << '\n';

	// 2. Read in from the input stream until the end of the file
	while (inputStream >> timestamp >> sell_buy >> temp >> trader >> temp >> stock >> temp >> price >> temp >> quantity) {
		// Error Check
		if (timestamp < 0 || trader >= num_traders || stock >= num_stocks || price <= 0 || quantity <= 0 || (timestamp < current_time))
		{
			cerr << "Error! Don't do that" << '\n';
			exit(1);
		}
		
		// 3. First determine if the new order's timestamp != current timestamp
		if (timestamp != current_time) {
			// a. Do something with median
			//////////////////////////////// MEDIAN OUTPUT /////////////////////////////////////////////////////////
			if (is_median)
			{
				for (size_t i = 0; i < stocks.size(); i++)
				{
					if (!stocks[i].left_heap.empty() || !stocks[i].right_heap.empty())
					{
						if (stocks[i].left_heap.size() == stocks[i].right_heap.size())
						{
							tempoo = (stocks[i].left_heap.top() + stocks[i].right_heap.top()) / 2;
						}
						else if (stocks[i].left_heap.size() > stocks[i].right_heap.size())
						{
							tempoo = stocks[i].left_heap.top();
						}
						else
						{
							tempoo = stocks[i].right_heap.top();
						}
						cout << "Median match price of Stock " << i << " at time " << current_time << " is $" << tempoo << '\n';
					}	
				}
				
			}
			//////////////////////////////// END OF MEDIAN OUTPUT //////////////////////////////////////////////////
			// b. Update our timestamp
			current_time = timestamp;
		}

		if (is_time)
		{
			//////////////////// TIME TRAVELER ////////////////////////////////////////////
			if (stocks[stock].traveler.trade_status == '0')
			{
				// For trade status 0, we simply need to find the first sell point
				if (sell_buy == "SELL")
				{
					stocks[stock].traveler.sell_price = price;
					stocks[stock].traveler.sell_timestamp = current_time;
					stocks[stock].traveler.trade_status = '1';
				}
			}
			else if (stocks[stock].traveler.trade_status == '1')
			{
				// For trade status 1, we need to determine if there are better sell prices(lower)
				// If theres a buy option thats better than the sell price, then we change the status
				if (sell_buy == "SELL" && price < stocks[stock].traveler.sell_price)
				{
					stocks[stock].traveler.sell_price = price;
					stocks[stock].traveler.sell_timestamp = current_time;
				}
				if (sell_buy == "BUY" && price > stocks[stock].traveler.sell_price)
				{
					stocks[stock].traveler.buy_price = price;
					stocks[stock].traveler.buy_timestamp = current_time;
					stocks[stock].traveler.trade_status = '2';
				}
			}
			else if (stocks[stock].traveler.trade_status == '2')
			{
				// For trade status 2, we need to determine if there are better buy prices
				// then we simply continue to update
				if (sell_buy == "BUY" && price > stocks[stock].traveler.buy_price)
				{
					stocks[stock].traveler.buy_price = price;
					stocks[stock].traveler.buy_timestamp = current_time;
				}
				// Determine if theres a sell that happens after you get a good buy
				// If this is the case, we go to the third state(potentially good)
				if (sell_buy == "SELL" && price < stocks[stock].traveler.sell_price)
				{
					stocks[stock].traveler.sell_temp = price;
					stocks[stock].traveler.sell_time_temp = current_time;
					stocks[stock].traveler.trade_status = '3';
				}
			}
			else if (stocks[stock].traveler.trade_status == '3')
			{
				// In this state we want to check for a buy price where it is much better
				// As well as better sell prices
				if (sell_buy == "SELL" && price < stocks[stock].traveler.sell_temp)
				{
					stocks[stock].traveler.sell_temp = price;
					stocks[stock].traveler.sell_time_temp = current_time;
				}
				if (sell_buy == "BUY" && ((price - stocks[stock].traveler.sell_temp) > (stocks[stock].traveler.buy_price - stocks[stock].traveler.sell_price)))
				{
					stocks[stock].traveler.buy_price = price;
					stocks[stock].traveler.buy_timestamp = current_time;
					stocks[stock].traveler.sell_price = stocks[stock].traveler.sell_temp;
					stocks[stock].traveler.sell_timestamp = stocks[stock].traveler.sell_time_temp;

					// Go back to the last timestamp
					stocks[stock].traveler.trade_status = '2';
				}
			}
			//////////////////// END OF TIME TRAVELER /////////////////////////////////////
		}

		// 4. Make all possible matches
		// Now we check this order with any previously posted orders
		// Access the stock corresponding to the stock # (0 -> 0, 1 -> 1, etc)
		// Check if sell_buy is buy or sell
		if (sell_buy == "BUY") {
			// First update with all the information
			buytemp.buy_trader = trader;
			buytemp.quantity_buy = quantity;
			buytemp.stock_buy = price;
			buytemp.index_buy = index;

			// Only push into the priority queue if there's nothing there or we cannot find anything in the queue that matches
			stocks[stock].buyer.push(buytemp);
		}
		else if (sell_buy == "SELL") {
			// First update with all the information
			selltemp.seller_trader = trader;
			selltemp.quantity_sell = quantity;
			selltemp.stock_sell = price;
			selltemp.index_sel = index;

			// Only push into the priority queue if there's nothing there or we cannot find anything in the queue that matches
			stocks[stock].seller.push(selltemp);
		}

		// If there is something in the buyer and seller priority queues then we check. Otherwise, theres nothing to match
		while (!stocks[stock].seller.empty() && !stocks[stock].buyer.empty()) {
			// If the top price is smaller than the buy price, then we remove that from the queue and 
			// return it if verbose is specified
			buytemp = stocks[stock].buyer.top();
			selltemp = stocks[stock].seller.top();
			if ((selltemp.stock_sell <= buytemp.stock_buy)) {

				int temp = 0;
				if (selltemp.index_sel < buytemp.index_buy) { temp = selltemp.stock_sell; }
				else { temp = buytemp.stock_buy; }
				int min_temp = min(buytemp.quantity_buy, selltemp.quantity_sell);

				if (is_verbose)
				{
					//////////////////// PRINT FOR VERBOSE /////////////////////////////////////////////
					cout << "Trader " << buytemp.buy_trader << " purchased " << min_temp << " shares of Stock " 
						<< stock << " from Trader " << selltemp.seller_trader << " for $" << temp << "/share" << '\n';
					//////////////////// END OF VERBOSE OUTPUT /////////////////////////////////////////
				}
				

				if (is_median)
				{
					//////////////////// MEDIAN INFO OUTPUT ////////////////////////////////////////////
					if (stocks[stock].counter == 0) { stocks[stock].left_heap.push(temp); }
					if (stocks[stock].counter == 1) {
						stocks[stock].right_heap.push(temp);
						if (stocks[stock].right_heap.top() < stocks[stock].left_heap.top()) {
							stocks[stock].left_heap.push(stocks[stock].right_heap.top());
							stocks[stock].right_heap.push(stocks[stock].left_heap.top());
							stocks[stock].left_heap.pop();
							stocks[stock].right_heap.pop();
						}
					}
					if (stocks[stock].counter > 1)
					{
						if (temp <= stocks[stock].left_heap.top())
						{
							stocks[stock].left_heap.push(temp);
						}
						else
						{
							stocks[stock].right_heap.push(temp);
						}
						// Now balance the two heaps
						if ((stocks[stock].left_heap.size() > stocks[stock].right_heap.size()) && (stocks[stock].left_heap.size()- stocks[stock].right_heap.size()) > 1)
						{
							stocks[stock].right_heap.push(stocks[stock].left_heap.top());
							stocks[stock].left_heap.pop();
						}
						if ((stocks[stock].right_heap.size() > stocks[stock].left_heap.size()) && (stocks[stock].right_heap.size() - stocks[stock].left_heap.size()) > 1)
						{
							stocks[stock].left_heap.push(stocks[stock].right_heap.top());
							stocks[stock].right_heap.pop();
						}
					}
					stocks[stock].counter += 1;
					//////////////////// END MEDIAN INFO OUTPUT ////////////////////////////////////////
				}

				if (is_trader)
				{
					//////////////////// TRADER INFO OUTPUT ////////////////////////////////////////////
					trader_info[buytemp.buy_trader].num_bought += min_temp;
					trader_info[selltemp.seller_trader].num_sold += min_temp;
					trader_info[buytemp.buy_trader].net_transfer -= (min_temp * temp);
					trader_info[selltemp.seller_trader].net_transfer += (min_temp * temp);
					//////////////////// END OF TRADER INFO OUTPUT /////////////////////////////////////
				}
				
				
				// Now we reduce the number of stocks that each buyer/seller possesses
				// If the seller stocks > buyer stocks, then we do not pop, simply change
				if (buytemp.quantity_buy < selltemp.quantity_sell) {
					// Reduce the number of stocks available for buying
					stocks[stock].seller.top().quantity_sell -= buytemp.quantity_buy;

					// All buy stocks are gone
					stocks[stock].buyer.pop();
				}
				// If the buyer had more stocks they wanted to buy than what was sold
				else if (buytemp.quantity_buy > selltemp.quantity_sell) {
					// Reduce the number of stocks that need to be bought
					stocks[stock].buyer.top().quantity_buy -= selltemp.quantity_sell;

					// All sell stocks are gone
					stocks[stock].seller.pop();
				}
				// If they are exactly the same
				else if (buytemp.quantity_buy == selltemp.quantity_sell) {
					stocks[stock].seller.pop();
					stocks[stock].buyer.pop();
				}
				counter += 1;
			}
			else
			{
				break;
			}
		}

		index += 1;
	}

	// If median is specified print here as well
	if (is_median)
	{
		for (size_t i = 0; i < stocks.size(); i++)
		{
			if (!stocks[i].left_heap.empty() || !stocks[i].right_heap.empty())
			{
				if (stocks[i].left_heap.size() == stocks[i].right_heap.size())
				{
					tempoo = (stocks[i].left_heap.top() + stocks[i].right_heap.top()) / 2;
				}
				else if (stocks[i].left_heap.size() > stocks[i].right_heap.size())
				{
					tempoo = stocks[i].left_heap.top();
				}
				else
				{
					tempoo = stocks[i].right_heap.top();
				}
				cout << "Median match price of Stock " << i << " at time " << current_time << " is $" << tempoo << '\n';
			}
		}

	}

	// End of day and orders processed
	cout << "---End of Day---" << '\n';
	cout << "Orders Processed: " << counter << '\n';

	if (is_trader) {
		// Trader Info output if specified
		cout << "---Trader Info---" << '\n';
		for (size_t i = 0; i < trader_info.size(); i++)
		{
			cout << "Trader " << i << " bought " << trader_info[i].num_bought << " and sold " << trader_info[i].num_sold
				<< " for a net transfer of $" << trader_info[i].net_transfer << '\n';
		}
	}

	if (is_time)
	{
		// Time travelers output if specifies
		cout << "---Time Travelers---" << '\n';
		for (size_t i = 0; i < stocks.size(); i++)
		{
			if (stocks[i].traveler.buy_price - stocks[i].traveler.sell_price > 0)
			{
				cout << "A time traveler would buy shares of Stock " << i << " at time " << stocks[i].traveler.sell_timestamp
					<< " for $" << stocks[i].traveler.sell_price << " and sell these shares at time " << stocks[i].traveler.buy_timestamp
					<< " for $" << stocks[i].traveler.buy_price << '\n';
			}
			else
			{
				cout << "A time traveler could not make a profit on shares of Stock " << i << '\n';
			}
		}
	}
}