#include <iostream>
#include <sqlite3.h>
#include <string>
#include <iomanip>
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
		//fprintf(stdout, "Records created successfully\n");
	}

	sqlite3_finalize(stmt);


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
	this->shippingAddress = new Address();
	this->billingAddress = new Address();
	this->card = new Card();
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
		this->shippingAddress->Name = sqlShipName;
	}
	char* sqlShipAdd1 = (char*)sqlite3_column_text(stmt, 4);
	if (sqlShipAdd1)
	{
		this->shippingAddress->AddressLine1 = sqlShipAdd1;
	}
	char* sqlShipAdd2 = (char*)sqlite3_column_text(stmt, 5);
	if (sqlShipAdd2)
	{
		this->shippingAddress->AddressLine2 = sqlShipAdd2;
	}
	char* sqlShipCity = (char*)sqlite3_column_text(stmt, 6);
	if (sqlShipCity)
	{
		this->shippingAddress->City = sqlShipCity;
	}
	char* sqlShipState = (char*)sqlite3_column_text(stmt, 7);
	if (sqlShipState)
	{
		this->shippingAddress->State = sqlShipState;
	}
	char* sqlShipZip = (char*)sqlite3_column_text(stmt, 8);
	if (sqlShipZip)
	{
		this->shippingAddress->Zip = sqlShipZip;
	}
	if (this->shippingAddress->Name != "" && this->shippingAddress->AddressLine1 != "" && this->shippingAddress->City != "" && this->shippingAddress->State != "" && this->shippingAddress->Zip != "")
	{
		this->hasValidShippingAddress = true;
	}


	char* sqlBillName = (char*)sqlite3_column_text(stmt, 14);
	if (sqlBillName)
	{
		this->billingAddress->Name = sqlBillName;
	}
	char* sqlBillAdd1 = (char*)sqlite3_column_text(stmt, 15);
	if (sqlBillAdd1)
	{
		this->billingAddress->AddressLine1 = sqlBillAdd1;
	}
	char* sqlBillAdd2 = (char*)sqlite3_column_text(stmt, 16);
	if (sqlBillAdd2)
	{
		this->shippingAddress->AddressLine2 = sqlBillAdd2;
	}
	char* sqlBillCity = (char*)sqlite3_column_text(stmt, 17);
	if (sqlBillCity)
	{
		this->billingAddress->City = sqlBillCity;
	}
	char* sqlBillState = (char*)sqlite3_column_text(stmt, 18);
	if (sqlBillState)
	{
		this->billingAddress->State = sqlBillState;
	}
	char* sqlBillZip = (char*)sqlite3_column_text(stmt, 19);
	if (sqlBillZip)
	{
		this->billingAddress->Zip = sqlBillZip;
	}

	if (this->billingAddress->Name != "" && this->billingAddress->AddressLine1 != "" && this->billingAddress->City != "" && this->billingAddress->State != "" && this->billingAddress->Zip != "")
	{
		this->hasValidBillingAddress = true;
	}

	char* sqlCardName = (char*)sqlite3_column_text(stmt, 9);
	if (sqlCardName)
	{
		this->card->cardHolder = sqlCardName;
	}
	char* sqlCardNumber = (char*)sqlite3_column_text(stmt, 10);
	if (sqlCardNumber)
	{
		this->card->cardNumber = sqlCardNumber;
	}
	char* sqlCardExpDate = (char*)sqlite3_column_text(stmt, 11);
	if (sqlCardExpDate)
	{
		this->card->cardExpDate = sqlCardExpDate;
	}
	char* sqlCardSecurityCode = (char*)sqlite3_column_text(stmt, 12);
	if (sqlCardSecurityCode)
	{
		this->card->cardSecurityCode = sqlCardSecurityCode;
	}

	if (this->card->cardHolder != "" && this->card->cardNumber != "" && this->card->cardExpDate != "" && this->card->cardSecurityCode != "")
	{
		this->hasValidCard = true;
	}


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
			sqlite3_finalize(stmt);
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

Address::Address(string name, string line1, string line2, string city, string state, string zip)
{
	this->Name = name;
	this->AddressLine1 = line1;
	this->AddressLine2 = line2;
	this->City = city;
	this->State = state;
	this->Zip = zip;

}


Address* Account::getShippingAddress()
{
	return this->shippingAddress;
}


Address* Account::getBillingAddress()
{
	return this->billingAddress;
}

void Account::setShippingAddress(Address* address)
{
	this->shippingAddress = address;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	string idStr = to_string(this->id);

	string accountShipAddUpdateSql = "UPDATE Account SET shippingName = '" + address->Name + "', ShippingAddressLine1 = '" + address->AddressLine1 + "', ShippingAddressLine2 = '" + address->AddressLine2 + "', ShippingCity = '" + address->City + "', ShippingZip = '" + address->Zip + "', ShippingState = '" + address->State + "' WHERE ID = " + idStr + "; ";
	rc = sqlite3_prepare(db, accountShipAddUpdateSql.c_str(), -1, &stmt, NULL);
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
	else
	{
		cout << endl << "Successfully Updated Shipping Address Information" << endl;

		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return;
	}
}

void Account::setBillingAddress(Address* address)
{
	this->billingAddress = address;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	string idStr = to_string(this->id);

	string accountBillAddUpdateSql = "UPDATE Account SET billingName = '" + address->Name + "', billingAddressLine1 = '" + address->AddressLine1 + "', billingAddressLine2 = '" + address->AddressLine2 + "', billingCity = '" + address->City + "', billingZip = '" + address->Zip + "', billingState = '" + address->State + "' WHERE ID = " + idStr + ";";
	rc = sqlite3_prepare(db, accountBillAddUpdateSql.c_str(), -1, &stmt, NULL);
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
	else
	{
		cout << endl << "Successfully Updated Billing Address Information" << endl;

		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return;
	}
}


void Account::displayEditNameEmail()
{
	string name = "";
	string email = "";
	cout << endl << "Please enter your Name: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, name);
	this->setName(name);
	cout << endl << "Please enter your Email: ";
	cin >> email;
	this->setEmail(email);
}

void Account::displayEditPassword()
{
	string oldPassword = "";
	string newPassword1 = "";
	string newPassword2 = "";
	bool validPasswords = false;

	while (!validPasswords)
	{
		cout << endl << "Please enter your old password: ";
		cin >> oldPassword;
		cout << endl << "Please enter your new password: ";
		cin >> newPassword1;
		cout << endl << "Please enter your new password again: ";
		cin >> newPassword2;

		if (newPassword1 != newPassword2)
		{
			cout << "Passwords don't match. Please try again.";
		}
		else
		{
			validPasswords = this->setPassword(oldPassword, newPassword1);
			if (!validPasswords)
			{
				cout << "Old password wasn't correct. Please try again.";
			}
			break;
		}
	}


}

void Account::displayEditAddressInfo()
{
	string name = "";
	string line1 = "";
	string line2 = "";
	string city = "";
	string state = "";
	string zip = "";
	cout << endl << "Please enter Shipping Name: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, name);
	cout << endl << "Please enter Shipping Address Line 1: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, line1);
	cout << endl << "Please enter Shipping Address Line 2: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, line2);
	cout << endl << "Please enter Shipping Address City: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, city);
	cout << endl << "Please enter Shipping Address State (Abbrev): ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, state);
	cout << endl << "Please enter Shipping Address Zip: ";
	cin >> zip;

	shippingAddress = new Address(name, line1, line2, city, state, zip);
	this->setShippingAddress(shippingAddress);


	cout << endl << "Please enter Billing Name: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, name);
	cout << endl << "Please enter Billing Address Line 1: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, line1);
	cout << endl << "Please enter Billing Address Line 2: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, line2);
	cout << endl << "Please enter Billing Address City: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, city);
	cout << endl << "Please enter Billing Address State (Abbrev): ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, state);
	cout << endl << "Please enter Billing Address Zip: ";
	cin >> zip;

	billingAddress = new Address(name, line1, line2, city, state, zip);


	this->setBillingAddress(billingAddress);
}

void Account::displayEditCardInfo()
{
	string name = "";
	string number = "";
	string expDate = "";
	string secCode = "";
	cout << endl << "Please enter Card Holder Name: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, name);
	cout << endl << "Please enter Card Number: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin >> number;
	cout << endl << "Please enter Card Expiration Date: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin >> expDate;
	cout << endl << "Please enter Card Security Code: ";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin >> secCode;

	card = new Card(name, number, expDate, secCode);
	this->setCard(card);
}

Card::Card(string name, string number, string date, string securityCode)
{
	this->cardHolder = name;
	this->cardNumber = number;
	this->cardExpDate = date;
	this->cardSecurityCode = securityCode;
}

Card* Account::getCard()
{
	return this->card;
}


bool Account::setPassword(string oldPassword, string newPassword)
{
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	string idStr = to_string(this->id);


	string passwordSelectSql = "SELECT * FROM Account WHERE ID = " + idStr + "; ";
	rc = sqlite3_prepare(db, passwordSelectSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		char* oldPasswordChar = (char*)sqlite3_column_text(stmt, 2);
		if (oldPasswordChar)
		{
			string oldPasswordStr(oldPasswordChar);
			if (oldPasswordStr == oldPassword)
			{
				sqlite3_reset(stmt);
				sqlite3_finalize(stmt);
				string accountPasswordUpdateSql = "UPDATE Account SET password = '" + newPassword + "' WHERE ID = " + idStr + "; ";
				rc = sqlite3_prepare(db, accountPasswordUpdateSql.c_str(), -1, &stmt, NULL);
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
				else
				{
					cout << endl << "Successfully Updated Password" << endl;
					sqlite3_reset(stmt);
					sqlite3_finalize(stmt);
					sqlite3_close(db);
					return true;
				}
			} 
			else
			{
				sqlite3_close(db);
				return false;
			}
			sqlite3_close(db);

		}
		sqlite3_close(db);

	}
	sqlite3_close(db);

}

void Account::setCard(Card* card)
{
	this->card = card;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	string idStr = to_string(this->id);

	string accountCardUpdateSql = "UPDATE Account SET cardHolder = '" + card->cardHolder + "', cardNumber = '" + card->cardNumber + "', cardExpDate = '" + card->cardExpDate + "', cardSecurityCode = '" + card->cardSecurityCode + "' WHERE ID = " + idStr + "; ";
	rc = sqlite3_prepare(db, accountCardUpdateSql.c_str(), -1, &stmt, NULL);
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
	else
	{
		cout << endl << "Successfully Updated Card Information" << endl;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return;
	}
}

void Account::setName(string name)
{
	this->name = name;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	string idStr = to_string(this->id);

	string accountNameUpdateSql = "UPDATE Account SET Name = '" + name + "' WHERE ID = " + idStr + "; ";
	rc = sqlite3_prepare(db, accountNameUpdateSql.c_str(), -1, &stmt, NULL);
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
	else
	{
		cout << endl << "Successfully Updated Name" << endl;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return;
	}
}

void Account::setEmail(string email)
{
	this->email = email;
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	string idStr = to_string(this->id);

	string accountEmailUpdateSql = "UPDATE Account SET Email = '" + email + "' WHERE ID = " + idStr + "; ";
	rc = sqlite3_prepare(db, accountEmailUpdateSql.c_str(), -1, &stmt, NULL);
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
	else
	{
		cout << endl << "Successfully Updated Email" << endl;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return;
	}
}

void Account::emptyCartReduceStock()
{
	this->currentCart = new Cart(this);
}

void Account::displayConfirm()
{
	Cart* currentCart = this->getCart();
	cout << endl;
	currentCart->displayCart();
	cout << endl;
	string input = "";
	bool confirmedOrder = false;

	while (!confirmedOrder)
	{
		cout << endl << "Do you want to confirm this order? (y/n)";
		cin >> input;
		if (input[0] == 'y')
		{
			Order* newOrder = new Order(this);
			this->orders.push_back(newOrder);

			this->emptyCartReduceStock();
			cout << endl << "Order Placed: Thank you for shopping with us!" << endl << endl << endl;
			break;
		}
	}




}

bool Account::hasValidAddresses()
{
	return (this->hasValidBillingAddress && this->hasValidShippingAddress);
}


vector<Order*> Account::getOrders()
{
	return this->orders;
}


void Account::displayPastOrders()
{
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	string accountIDStr = to_string(this->getID());

	string pastOrdersSelectSql = "SELECT * FROM Orders WHERE Account = " + accountIDStr + "; ";
	rc = sqlite3_prepare(db, pastOrdersSelectSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);
	this->orders.clear();
	while (sqlite3_column_text(stmt, 1))
	{
		Order* tempOrder = new Order();
		char* orderIDChar = (char*)sqlite3_column_text(stmt, 0);
		int orderID = stoi(orderIDChar);
		tempOrder->setOrder_Number(orderID);
		char* dateChar = (char*)sqlite3_column_text(stmt, 3);
		tempOrder->setOrder_Date(dateChar);
		char* totalPriceChar = (char*)sqlite3_column_text(stmt, 4);
		float totalPrice = strtof(totalPriceChar, NULL);
		tempOrder->setTotalPrice(totalPrice);
		this->orders.push_back(tempOrder);

		rc = sqlite3_step(stmt);

	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	cout << endl << "---> Past Orders <---" << endl;
	vector<Order*>::iterator itr;
	vector<Order*>pastOrders = this->getOrders();
	int counter = 1;
	for (itr = pastOrders.begin(); itr != pastOrders.end(); itr++)
	{
		cout <<  "Order Number: " << (*itr)->getOrder_Number() << right << setw(6) << "$" << fixed << setprecision(2) << (*itr)->getTotalPrice() << endl;
	}
	cout << endl;
}

bool Account::deleteAccount()
{
	string input = "";
	bool deleteConfirmed = false;

	while (!deleteConfirmed)
	{
		cout << endl << "Are you sure you want to delete your account? (y/n): ";
		cin >> input;
		if (input[0] != 'y')
		{
			return false;
		}
		else
		{
			sqlite3* db = nullptr;
			sqlite3_stmt* stmt = nullptr;
			char* zErrMsg = 0;
			int rc;
			db = getDBConnection(DB_NAME, db);
			string idStr = to_string(this->id);

			string deleteAccountSql = "DELETE FROM Account WHERE ID = " + idStr + ";";

			rc = sqlite3_prepare(db, deleteAccountSql.c_str(), -1, &stmt, NULL);
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
			else
			{
				cout << endl << "Successfully Deleted Account" << endl;
				sqlite3_reset(stmt);
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return true;
			}
		}
	}
	
}