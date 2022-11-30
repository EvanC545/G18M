#pragma once
#include <string>
#include "cart.h"
#include "order.h"
using std::string;


class Card {
public:
	string cardHolder;
	string cardNumber;
	string cardExpDate;
	string cardSecurityCode;
	Card() {};
	Card(string name, string number, string date, string securityCode);
};


class Address {
public:
	string Name;
	string AddressLine1;
	string AddressLine2;
	string City;
	string State;
	string Zip;
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
	bool hasValidShippingAddress = false;
	Address* billingAddress;
	bool hasValidBillingAddress = false;
	vector<Order*> orders;
	Card* card;
	bool hasValidCard = false;
	Cart* currentCart;

public:
	Account() {};
	Account(string name, string email, string password);
	Account(int id);

	void setName(string name);
	string getName();
	void setEmail(string email);
	string getEmail();
	bool setPassword(string oldPassword, string newPassword);


	void setShippingAddress(Address* address);
	Address* getShippingAddress();
	void setBillingAddress(Address* address);
	Address* getBillingAddress();
	bool hasValidAddresses();
	void setCard(Card* card);
	Card* getCard();
	void setCart(Cart* cart);
	int getID();
	Cart* getCart();
	vector<Order*> getOrders();
	void displayEditNameEmail();
	void displayEditPassword();
	void displayEditAddressInfo();
	void displayEditCardInfo();
	void displayConfirm();
	void displayPastOrders();
	void emptyCartReduceStock();
	bool deleteAccount();

};



class AccountManager {
public:
	AccountManager() {};
	int authenticate(string email, string password);
};