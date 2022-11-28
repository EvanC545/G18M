#include <iostream>
#include <sqlite3.h>
#include "account.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

Account::Account(string name, string email, string password)
{
	sqlite3* db;
	char* zErrMsg = 0;

	int rc = sqlite3_open("identifier.sqlite", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
	string accountSql = "INSERT INTO INSERT INTO Account (Name, Email, password, shippingName, ShippingAddressLine1, ShippingAddressLine2, ShippingCity, ShippingState, ShippingZip, cardHolder, cardNumber, cardExpDate, cardSecurityCode) VALUES('Connor Carr', 'cjclmemphis', 'password123', null, null, null, null, null, null, null, null, null, null); ";

	accountSql = "INSERT INTO Account (Name, Email, password, shippingName, ShippingAddressLine1, ShippingAddressLine2, ShippingCity, ShippingState, ShippingZip, cardHolder, cardNumber, cardExpDate, cardSecurityCode) VALUES('" + name;

	rc = sqlite3_exec(db, accountSql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Records created successfully\n");
	}

	sqlite3_close(db);

}