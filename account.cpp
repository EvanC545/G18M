#include <iostream>
#include <sqlite3.h>
#include <string>
#include "account.h"

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


Account::Account(string name, string email, string password)
{
	this->name = name;
	this->email = email;
	this->password = password;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);


	// MOVE THIS CODE TO BEFORE THE CREATION OF ACCOUNT
	//string accountExistsSql = "SELECT * FROM Account where Email = '" + this->email + "';";
	//rc = sqlite3_prepare(db, accountExistsSql.c_str(), -1, &stmt, NULL);
	//if (rc != SQLITE_OK)
	//{
	//	fprintf(stderr, "SQL Statement error: %s\n", &zErrMsg);
	//	sqlite3_free(zErrMsg);
	//}
	//rc = sqlite3_step(stmt);
	//char* str = (char*)sqlite3_column_text(stmt, 13);

	//if (rc != SQLITE_ROW) {
	//	fprintf(stderr, "Record not found");
	//	sqlite3_free(zErrMsg);
	//}
	//else {
	//	fprintf(stderr, "Account already exists with that email");}


	string accountSql = "INSERT INTO Account(Name, Email, password, shippingName, ShippingAddressLine1, ShippingAddressLine2, ShippingCity, ShippingState, ShippingZip, cardHolder, cardNumber, cardExpDate, cardSecurityCode) VALUES('" + name + "', '" + email + "', '" + password + "', null, null, null, null, null, null, null, null, null, null); ";

	const char* accountSqlChar = accountSql.c_str();
	rc = sqlite3_prepare(db, accountSqlChar, -1, &stmt, NULL);
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

	string accountSelectSql = "SELECT * FROM Account where Email = '" + this->email + "';";

	rc = sqlite3_prepare(db, accountSelectSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	rc = sqlite3_step(stmt);
	char* str = (char*)sqlite3_column_text(stmt, 13);

	if (rc != SQLITE_ROW) {
		fprintf(stderr, "Record not found");
		sqlite3_free(zErrMsg);
	}
	else {
		this->id = strtol(str, NULL, 10);
		//fprintf(stdout, "Record: %s\n", str);
	}
	sqlite3_finalize(stmt);

	sqlite3_close(db);
}


Account::Account(int id)
{
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);

	string accountSelectSql = "SELECT * FROM Account where ID = '" + std::to_string(id) + "';";
	rc = sqlite3_prepare(db, accountSelectSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		fprintf(stderr, "Account doesn't exist with that ID");
		sqlite3_free(zErrMsg);
	}
	this->id = id;
	char* sqlEmail = (char*)sqlite3_column_text(stmt, 1);
	this->email = sqlEmail;
	char* sqlName = (char*)sqlite3_column_text(stmt, 0);
	this->name = sqlName;
	char* sqlShipName = (char*)sqlite3_column_text(stmt, 3);
	if (sqlShipName)
	{
		this->shippingName = sqlShipName;
	}
	char* sqlShipAdd1 = (char*)sqlite3_column_text(stmt, 4);
	if (sqlShipAdd1)
	{
		this->shippingAddressLine1 = sqlShipAdd1;
	}
	char* sqlShipAdd2 = (char*)sqlite3_column_text(stmt, 5);
	if (sqlShipAdd2)
	{
		this->shippingAddressLine2 = sqlShipAdd2;
	}
	char* sqlShipCity = (char*)sqlite3_column_text(stmt, 6);
	if (sqlShipCity)
	{
		this->shippingCity = sqlShipCity;
	}
	char* sqlShipState = (char*)sqlite3_column_text(stmt, 7);
	if (sqlShipState)
	{
		this->shippingState = sqlShipState;
	}
	char* sqlShipZip = (char*)sqlite3_column_text(stmt, 8);
	if (sqlShipZip)
	{
		this->shippingZip = sqlShipZip;
	}
	
	// TODO: Need Card Details
	sqlite3_finalize(stmt);

	sqlite3_close(db);

}


int AccountManager::authenticate(string email, string password)
{
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);


	string accountEmailSql = "SELECT * FROM Account where Email = '" + email + "';";

	rc = sqlite3_prepare(db, accountEmailSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);

	if (rc != SQLITE_ROW) {
		fprintf(stderr, "Account doesn't exist with that email");
		sqlite3_free(zErrMsg);
	}
	else {
		char* sqlEmail = (char*)sqlite3_column_text(stmt, 1);
		if (email == sqlEmail)
		{
			char* sqlPassword = (char*)sqlite3_column_text(stmt, 2);
			if (password == sqlPassword)
			{
				std::cout << std::endl << "Login Successful" << std::endl;
				char* accountIDChar = (char*)sqlite3_column_text(stmt, 13);
				int id = strtol(accountIDChar, NULL, 10);
				sqlite3_finalize(stmt);
				sqlite3_close(db);

				return id;
			}
			std::cout << "Login Invalid" << std::endl;
			sqlite3_close(db);

			return 0;
		}
		sqlite3_close(db);

		return 0;
	}
	sqlite3_close(db);

	return 0;
}


string Account::getName()
{
	return this->name;
}

int Account::getID()
{
	return this->id;
}

void Account::setCart(Cart* cart)
{
	this->currentCart = cart;
}

Cart* Account::getCart()
{
	return this->currentCart;
}
