// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#pragma once
#include <string>
#include <queue>
#include <vector>
using namespace std;

// Make another struct to keep the other variables, in this case Buyers
struct Buyer
{
	Buyer() : buy_trader(0), stock_buy(0), quantity_buy(0), index_buy(0) {}
	// Buyer will be some sort of trader
	unsigned int buy_trader;

	// Buyer will also request some stock at some price
	int stock_buy;

	// Probably also has some quantity
	mutable unsigned int quantity_buy;

	// Also has the index at which they bought
	unsigned int index_buy;

};

struct Seller
{
	Seller() : seller_trader(0), stock_sell(0), quantity_sell(0), index_sel(0) {}
	// Seller will be some sort of trader
	unsigned int seller_trader;

	// Seller will also be selling some stock at some price
	int stock_sell;

	// Probably has some quantity
	mutable unsigned int quantity_sell;
	
	// Also has the index at which they bought
	unsigned int index_sel;

};

// Our functor to overload buyer
struct Compare_Buyer
{
	bool operator()(const Buyer & t1, const Buyer & t2) const
	{
		// First check the time_stamp, if that is the same,
		// break the tie with the buying price
		
		if (t1.stock_buy == t2.stock_buy)
		{
			// If they have the same timestamp and buy value, return the previous one
			return t1.index_buy > t2.index_buy;
		}
		return t1.stock_buy < t2.stock_buy;
	}
};

// Our functor to overload buyer
struct Compare_Seller
{
	bool operator()(const Seller& t1, const Seller& t2) const
	{
		if (t1.stock_sell == t2.stock_sell)
		{
			// We want to return the lower index
			return t1.index_sel > t2.index_sel;
		}
		return t1.stock_sell > t2.stock_sell; 
	}
};

struct Time_Traveler
{
	Time_Traveler() : trade_status('0'), buy_price(0), buy_timestamp(0), sell_price(0), sell_timestamp(0), sell_temp(0), sell_time_temp(0) {}
	// 1. If there is no one selling, trade_status = '0'
	// 2. If there is someone willing to sell, trade_status = '1'
	// 3. If there is someone willing to buy at equal or higher sell price update buy price, trade_status = '2'
	// 4. If there is a better sell price, update sell price and trade_status = '3'
	// 5. If we are in trade_status = '3' and there is a better buy price(higher than current), go back to
	// trade_status = '2' and update buy price
	// 6. If day ends and we are in '3' or '2' then that is price we buy and sell at
	char trade_status;

	int buy_price;
	int buy_timestamp;
	int sell_price;
	int sell_timestamp;
	int sell_temp;
	int sell_time_temp;
};

// Create a Stock with underlying priority queue 
struct Stock
{
	Stock() : counter(0) {}
	
	// Need to keep track of median for certain timestamp, two priority queues
	priority_queue<int, vector<int>, less<int>> left_heap;
	priority_queue<int, vector<int>, greater<int>> right_heap;

	unsigned int counter;

	// For buyers, you want the one who will buy at lowest price, if there is a higher buy price, it goes to bottom
	priority_queue<Buyer, vector<Buyer>, Compare_Buyer> buyer;
	
	// For sellers, you want the one who will sell at the highest rate
	priority_queue<Seller, vector<Seller>, Compare_Seller> seller;

	Time_Traveler traveler;
};


struct Trader_info
{
	Trader_info() : num_bought(0), num_sold(0), net_transfer(0) {}
	
	int num_bought;
	int num_sold;
	int net_transfer;
};


