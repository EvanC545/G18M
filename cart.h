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
	string viewCart(string item);
	bool cartIsEmpty();
	void addItem(string item);
	void decItem(string item, bool cartIsEmpty);
	void removeItem(string item);
	double calcSubtotal(double itemPrice);
	void checkout(string item, double calcSubtotal);
};