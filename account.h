#pragma once
#include <string>
#include "cart.h"
using std::string;



class Address {
private:
	string Name;
	string AddressLine1;
	string AddressLine2;
	string City;
	string State;
	string Zip;
public:
	Address() {};
	Address(string name, string line1, string line2, string city, string state, string zip);
};

class Account {
private:
	bool dbFound = false;
	int id;
	string name;
	string email;
	string password;
	Address* shippingAddress;
	Address* billingAddress;
	string cardHolder;
	string cardNumber;
	string cardExpDate;
	string cardSecurityCode;
	Cart* currentCart;

public:
	Account() {};
	Account(string name, string email, string password);
	Account(int id);

	void setName(string name);
	string getName();
	void setEmail(string email);
	string getEmail();
	void setPassword(string oldPassword, string newPassword);
	bool authenticate(string email, string password);
	void setShippingAddress(string address);
	string getShippingAddress();
	void setBillingAddress(string address);
	string getBillingAddress();
	void setCard(string currentCardId);
	// Datatype?--> string getCard();
	void setCart(Cart* cart);
	// Datatype? --> string getCart();
	int getID();
	Cart* getCart();
};



class AccountManager {
public:
	AccountManager() {};
	int authenticate(string email, string password);
};