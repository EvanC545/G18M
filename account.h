#pragma once
#include <string>
#include "cart.h"
using std::string;



class Account {
private:
	bool dbFound = false;
	int id;
	string name;
	string email;
	string password;
	string shippingName;
	string shippingAddressLine1;
	string shippingAddressLine2;
	string shippingCity;
	string shippingState;
	string shippingZip;
	string cardHolder;
	string cardNumber;
	string cardExpDate;
	string cardSecurityCode;
	int currentCartId;

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
	void setCart(int cartId);
	// Datatype? --> string getCart();
	int getID();
};



class AccountManager {
public:
	AccountManager() {};
	int authenticate(string email, string password);
};