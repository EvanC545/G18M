#include <iostream>
#include <string>
#include "order.h"

void Order::setCustomer_Name(Account customer) {

	this->customer = customer;

};

string Order::getCustomer_Name() {

	return customer.getName();

};


void Order::setOrder_Number(int o_number) {

	Order_Number = o_number;

};

int Order::getOrder_Number() {

	return Order_Number;

};

void Order::setOrder_Date(int o_date) {

	Order_Date = o_date;

};

int Order::getOrder_Date() {

	return Order_Date;

};

void Order::setShipped_Date(int s_date) {

	Shipped_Date = s_date;

};

int Order::getShipped_Date() {

	return Shipped_Date;

};