#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sqlite3.h>
#include "cart.h"
#include "account.h"


using namespace std;
//constructors definitions

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

Cart::Cart(Account* account)
{
	this->account = account;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);

	int accountID = account->getID();
	string accountIDStr = to_string(accountID);
	string cartSql = "INSERT INTO Cart(Account) VALUES(" + accountIDStr + ");";

	rc = sqlite3_prepare(db, cartSql.c_str(), -1, &stmt, NULL);
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
	else {
		fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_finalize(stmt);


	string cartSelectSql = "SELECT * FROM Cart where Account = " + accountIDStr + " ORDER BY ID DESC;";
	rc = sqlite3_prepare(db, cartSelectSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	rc = sqlite3_step(stmt);
	char* str = (char*)sqlite3_column_text(stmt, 3);
	if (rc != SQLITE_ROW) {
		fprintf(stderr, "Record not found");
		sqlite3_free(zErrMsg);
	}
	else {
		this->id = stoi(str);
		//fprintf(stdout, "Record: %s\n", str);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);

}


bool Cart::addItem(Movie* movie)
{
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	int movieID = movie->getID();
	string movieIDStr = to_string(movieID);
	int cartID = this->id;
	string cartIDStr = to_string(cartID);

	string cartItemSql = "INSERT INTO CartItems(CartID, Item) VALUES(" + cartIDStr + ", " + movieIDStr + ");";

	rc = sqlite3_prepare(db, cartItemSql.c_str(), -1, &stmt, NULL);
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
	else {
		fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return false;
}


int Cart::getID()
{
	return this->id;
}