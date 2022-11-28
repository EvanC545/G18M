#pragma once
#include <string>
#include "account.h"
using std::string;

class Order {

private:

	Account customer;
	int Order_Number;
	int Order_Date;
	int Shipped_Date;

public:

	void setCustomer_Name(Account customer);
	string getCustomer_Name();
	int getCustomer_Number();
	void setOrder_Number(int o_number);
	int getOrder_Number();
	void setOrder_Date(int o_date);
	int getOrder_Date();
	void setShipped_Date(int s_date);
	int getShipped_Date();

};