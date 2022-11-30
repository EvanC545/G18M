#include <iostream>
#include <string>
#include <sqlite3.h>
#include <iterator>
#include <vector>
#include "account.h"
#include "order.h"

using namespace std;

extern string DB_NAME;



static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static sqlite3* getDBConnection(string Name, sqlite3* db)
{
	int rc = sqlite3_open(Name.c_str(), &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		//fprintf(stderr, "Opened database successfully\n");
	}
	return db;
}

bool addOrderItem(int orderId, Movie* movie)
{
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);

	string movieIDStr = to_string(movie->getID());
	string movieQuantityStr = to_string(movie->getQuantity());
	string moviePriceStr = to_string(movie->getPrice());
	string orderIdStr = to_string(orderId);

	string orderItemSql = "INSERT INTO OrderItems(Item, Quantity, ItemPrice, OrderID) VALUES(" + movieIDStr + ", " + movieQuantityStr + ", " + moviePriceStr + ", " + orderIdStr + "); ";
	rc = sqlite3_prepare(db, orderItemSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return true;
}



Order::Order(Account* account)
{
	this->account = account;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);

	int accountID = account->getID();
	string accountIDStr = to_string(accountID);
	Cart* accountCart = account->getCart();

	float totalPrice = accountCart->calcSubtotal();
	// Assuming no tax -- this is where it would go
	accountCart->setTotalCost(totalPrice);
	string totalPriceStr = to_string(totalPrice);



	string orderSql = "INSERT INTO Orders(Account, TotalPrice) VALUES(" + accountIDStr + ", " + totalPriceStr + "); ";
	rc = sqlite3_prepare(db, orderSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	string cartSelectSql = "SELECT * FROM Orders where Account = " + accountIDStr + " ORDER BY ID DESC;";
	rc = sqlite3_prepare(db, cartSelectSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	rc = sqlite3_step(stmt);
	char* orderIDChar = (char*)sqlite3_column_text(stmt, 0);
	if (rc != SQLITE_ROW) {
		fprintf(stderr, "Record not found");
		sqlite3_free(zErrMsg);
	}
	else {
		this->Order_Number = stoi(orderIDChar);
		//fprintf(stdout, "Record: %s\n", str);
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	vector<Movie*>::iterator itr;
	vector<Movie*> movies = accountCart->getMovies();
	for (itr = movies.begin(); itr != movies.end(); itr++)
	{
		addOrderItem(this->Order_Number, *itr);
	}
}


void Order::setOrder_Number(int orderId)
{
	this->Order_Number = orderId;
}

void Order::setOrder_Date(string orderDate)
{
	this->Order_Date = orderDate;
}


void Order::setTotalPrice(float total)
{
	this->totalPrice = total;
}

int Order::getOrder_Number()
{
	return this->Order_Number;
}

float Order::getTotalPrice()
{
	return this->totalPrice;
}