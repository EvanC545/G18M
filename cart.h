#pragma once
#include <string>
#include <vector>
#include "movie.h"

using namespace std;

class Account;

class Cart {
	
private:
	// Could use map to store Item : Quantity Pairs if wanted
	int numItems;
	Account* account;
	vector<Movie*> movies;
	//Movie* movie;
	float totalCost;
	float subTotal;
	

	
public:
	
	// Constructors
	Cart(Account* account);

	void viewCart();
	bool cartIsEmpty();
	//void addItem(Movie item);
	//void decItem(Movie item);
	//void removeItem(Movie item);
	void checkout();
	double calcSubtotal();
};
