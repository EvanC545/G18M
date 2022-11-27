#pragma once
#include <string>
#include <vector>

using std::string;

class Cart {
	
private:
	string tempItem;
	vector<string> item;
	vector<double> itemPrice;
	//int numItems;
	//double totalCost;
	//double subTotal;
	
	//Regular functions
	void viewCart(vector<string> item, vector<double> itemPrice);
	bool cartIsEmpty(vector<string> item);
	void addItem(vector<string> item, vector<double> itemPrice, string tempItem);
	void decItem(vector<string> item, vector<double> itemPrice, string tempItem);
	void removeItem(vector<string> item, vector<double> itemPrice, string tempItem);
	void checkout(vector<string> item, vector<double> itemPrice);
	double calcSubtotal(vector<double> itemPrice);
	
public:
	
	// Constructors
	Cart();
	//Cart(string c_item, double price); // int numItems, double c_cost
	//	   1		2		3		4
	// 1) Vector of items in cart
	// 2) Vector of prices of items in cart
	// 3) Number of items in cart (unused)
	// 4) Sum of all prices in price array (unused)
	
	// Public facing functions
	void viewCart();
	bool cartIsEmpty();
	void addItem(string tempItem);
	void decItem(string tempItem);
	void removeItem(string tempItem);
	void checkout();
	double calcSubtotal();
};
