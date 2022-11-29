#pragma once
#include <string>
#include <vector>
#include "movie.h"

using namespace std;

class Account;

class Cart {
	
private:
	// Could use map to store Item : Quantity Pairs if wanted
	int id;
	int numItems = 0;
	Account* account;
	vector<Movie*> movies;
	//Movie* movie;
	float totalCost = 0.00;
	float subTotal = 0.00;
	

	
public:
	
	// Constructors
	Cart(Account* account);

	void setSubtotal(float subTotal);
	float getSubtotal();
	void viewCart();
	bool cartIsEmpty();
	bool addItem(Movie* movie);
	bool decItem(Movie* movie);
	bool removeItem(Movie* movie);
	bool checkout();
	float calcSubtotal();
	int getID();
	void displayCart();
	void displayAddToCart();
	void displayRemoveFromCart();
};
