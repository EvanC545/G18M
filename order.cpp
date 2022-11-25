#include <iostream>
#include "order.h"

void Order::setCustomer_Name(string name) {

	Customer_Name = name;

};

string Order::getCustomer_Name() {

	return Customer_Name;

};

void Order::setCustomer_Number(int c_number) {

	Customer_Number = c_number;

};

int Order::getCustomer_Number() {

	return Customer_Number;

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