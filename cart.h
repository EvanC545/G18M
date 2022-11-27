#pragma once
#include <string>

using std::string;

class Cart {
	
private:
	
	string item;
	double itemPrice;
	int numItems;
	double totalCost;
	
public:
	
	// Constructors
	Cart();
	Cart(string c_item, double price, int numItems, double c_cost);
	//	1		2		3		4
	// 1) Array of items in cart
	// 2) Array of prices of items in cart
	// 3) Number of items in cart
	// 4) Sum of all prices in price array
	
	string viewCart(string item);
	bool cartIsEmpty();
	void addItem(string item);
	void decItem(string item, bool cartIsEmpty);
	void removeItem(string item);
	double calcSubtotal(double itemPrice);
	void checkout(string item, double calcSubtotal);
};
