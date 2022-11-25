#pragma once
#include <string>

using std::string;

class Order {

private:

	string Customer_Name;
	int Customer_Number;
	int Order_Number;
	int Order_Date;
	int Shipped_Date;

public:

	void setCustomer_Name(string name);
	string getCustomer_Name();
	void setCustomer_Number(int c_number);
	int getCustomer_Number();
	void setOrder_Number(int o_number);
	int getOrder_Number();
	void setOrder_Date(int o_date);
	int getOrder_Date();
	void setShipped_Date(int s_date);
	int getShipped_Date();

};