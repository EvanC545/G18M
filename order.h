#pragma once
#include <string>
using std::string;

class Account;

class Order {

private:
	Account* account;
	int Order_Number;
	string Order_Date;
	string Shipped_Date;
	float totalPrice;

public:

	Order() {};
	Order(Account* account);
	void setCustomer(Account* account);
	string getCustomer_Name();
	int getCustomer_Number();
	void setOrder_Number(int o_number);
	int getOrder_Number();
	void setOrder_Date(string orderDate);
	int getOrder_Date();
	void setShipped_Date(int s_date);
	int getShipped_Date();
	float getTotalPrice();
	void setTotalPrice(float total);
};