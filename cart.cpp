#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sqlite3.h>
#include <iomanip>
#include <map>
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



	string checkItemInSql = "SELECT * FROM CartItems where CartID = " + cartIDStr + " and Item = " + movieIDStr + ";";
	rc = sqlite3_prepare(db, checkItemInSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);
	// IF CART DOESN'T HAVE THAT ITEM ALREADY IN IT
	if (rc != SQLITE_ROW) {
		sqlite3_free(zErrMsg);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);


		int movieQuantity = movie->incrementQuantity();
		float moviePrice = movie->getPrice();
		string moviePriceStr = to_string(moviePrice);
		string cartItemSql = "INSERT INTO CartItems(CartID, Item, ItemPrice) VALUES(" + cartIDStr + ", " + movieIDStr + ", " + moviePriceStr + "); ";
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
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);

			this->numItems++;
			string cartNumOfItemsStr = to_string(this->numItems);
			this->movies.push_back(movie);
			float cartSubTotal = this->calcSubtotal();
			string cartSubTotalStr = to_string(cartSubTotal);

			string cartUpdateSql = "UPDATE Cart SET NumOfItems = " + cartNumOfItemsStr + ", TotalCost = " + cartSubTotalStr + " WHERE ID = " + cartIDStr + ";";
			rc = sqlite3_prepare(db, cartUpdateSql.c_str(), -1, &stmt, NULL);
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
				sqlite3_reset(stmt);
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return true;
			}
		}
		sqlite3_close(db);
	}
	// IF CART HAS THAT MOVIE IN IT
	else
	{
		char* cartItemIdChar = (char*)sqlite3_column_text(stmt, 0);
		int cartItemId = stoi(cartItemIdChar);
		string cartItemIdStr = to_string(cartItemId);
		
		char* cartItemQuantityChar = (char*)sqlite3_column_text(stmt, 3);

		int cartItemQuantity = stoi(cartItemQuantityChar);
		movie->setQuantity(cartItemQuantity);
		int newCartItemQuantity = movie->incrementQuantity();
		string cartItemQuantityStr = to_string(newCartItemQuantity);

		float cartItemPrice = movie->getPrice();
		string cartItemPriceStr = to_string(cartItemPrice);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		string cartItemUpdateSql = "UPDATE CartItems SET Quantity = " + cartItemQuantityStr + ", ItemPrice = " + cartItemPriceStr + " WHERE ID = " + cartItemIdStr + " and CartID = " + cartIDStr + " and Item =" + movieIDStr + ";";
		rc = sqlite3_prepare(db, cartItemUpdateSql.c_str(), -1, &stmt, NULL);
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
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);

			this->numItems++;
			string cartNumOfItemsStr = to_string(this->numItems);
			// Need to update Movie Vector
			vector<Movie*>::iterator itr;
			for (itr = this->movies.begin(); itr != this->movies.end(); itr++)
			{
				int currentId = (*itr)->getID();
				if (currentId == movieID)
				{
					(*itr)->setQuantity(newCartItemQuantity);
				}
			}

			float cartSubTotal = this->calcSubtotal();
			string cartSubTotalStr = to_string(cartSubTotal);

			string cartUpdateSql = "UPDATE Cart SET NumOfItems = " + cartNumOfItemsStr + ", TotalCost = " + cartSubTotalStr + " WHERE ID = " + cartIDStr + ";";
			rc = sqlite3_prepare(db, cartUpdateSql.c_str(), -1, &stmt, NULL);
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
				sqlite3_reset(stmt);
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return true;
			}
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
			sqlite3_close(db);

			return true;
		}
		sqlite3_close(db);

	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);


	sqlite3_close(db);



	return false;
}

bool Cart::decItem(Movie* movie)
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

	string checkItemInSql = "SELECT * FROM CartItems where CartID = " + cartIDStr + " and Item = " + movieIDStr + ";";
	rc = sqlite3_prepare(db, checkItemInSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);
	// IF CART DOESN'T HAVE THAT ITEM ALREADY IN IT
	if (rc != SQLITE_ROW) {
		sqlite3_free(zErrMsg);
		
	}
	// IF CART HAS THAT MOVIE IN IT
	else
	{
		char* cartItemIdChar = (char*)sqlite3_column_text(stmt, 0);
		int cartItemId = stoi(cartItemIdChar);
		string cartItemIdStr = to_string(cartItemId);
		char* cartItemQuantityChar = (char*)sqlite3_column_text(stmt, 3);
		int cartItemQuantity = stoi(cartItemQuantityChar);
		movie->setQuantity(cartItemQuantity);
		int newCartItemQuantity = movie->decrementQuantity();
		string cartItemQuantityStr = to_string(newCartItemQuantity);
		float cartItemPrice = movie->getPrice();
		string cartItemPriceStr = to_string(cartItemPrice);



		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);

		if (cartItemQuantity == 1)
		{
			string deleteCartItemSql = "DELETE FROM CartItems where ID = " + cartItemIdStr;
			rc = sqlite3_prepare(db, deleteCartItemSql.c_str(), -1, &stmt, NULL);
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
				sqlite3_reset(stmt);
				sqlite3_finalize(stmt);

				this->numItems--;
				string cartNumOfItemsStr = to_string(this->numItems);
				vector<Movie*>::iterator itr;
				for (itr = this->movies.begin(); itr != this->movies.end(); itr++)
				{
					int currentId = (*itr)->getID();
					if (currentId == movieID)
					{
						(*itr)->setQuantity(newCartItemQuantity);
					}
				}
				float cartSubTotal = this->calcSubtotal();
				string cartSubTotalStr = to_string(cartSubTotal);

				string cartUpdateSql = "UPDATE Cart SET NumOfItems = " + cartNumOfItemsStr + ", TotalCost = " + cartSubTotalStr + " WHERE ID = " + cartIDStr + ";";
				rc = sqlite3_prepare(db, cartUpdateSql.c_str(), -1, &stmt, NULL);
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
					sqlite3_reset(stmt);
					sqlite3_finalize(stmt);
					sqlite3_close(db);
					return true;
				}
			}
		}
		else
		{
			string cartItemUpdateSql = "UPDATE CartItems SET Quantity = " + cartItemQuantityStr + ", ItemPrice = " + cartItemPriceStr + " WHERE ID = " + cartItemIdStr + " and CartID = " + cartIDStr + " and Item =" + movieIDStr + ";";
			rc = sqlite3_prepare(db, cartItemUpdateSql.c_str(), -1, &stmt, NULL);
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
				sqlite3_reset(stmt);
				sqlite3_finalize(stmt);

				this->numItems--;
				string cartNumOfItemsStr = to_string(this->numItems);
				vector<Movie*>::iterator itr;
				for (itr = this->movies.begin(); itr != this->movies.end(); itr++)
				{
					if ((*itr)->getID() == movieID)
					{
						(*itr)->setQuantity(newCartItemQuantity);
					}
				}
				float cartSubTotal = this->calcSubtotal();
				string cartSubTotalStr = to_string(cartSubTotal);

				string cartUpdateSql = "UPDATE Cart SET NumOfItems = " + cartNumOfItemsStr + ", TotalCost = " + cartSubTotalStr + " WHERE ID = " + cartIDStr + ";";
				rc = sqlite3_prepare(db, cartUpdateSql.c_str(), -1, &stmt, NULL);
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
					sqlite3_reset(stmt);
					sqlite3_finalize(stmt);
					sqlite3_close(db);
					return true;
				}
				sqlite3_reset(stmt);
				sqlite3_finalize(stmt);
				sqlite3_close(db);

				return true;
			}
			sqlite3_close(db);
		}
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return false;
	}
}




int Cart::getID()
{
	return this->id;
}

void Cart::setSubtotal(float subTotal)
{
	this->subTotal = subTotal;
}

float Cart::getSubtotal()
{
	return this->subTotal;
}


float Cart::calcSubtotal()
{
	vector<Movie*>::iterator itr;
	float tempSubtotal = 0.00;

	for (itr = this->movies.begin(); itr != this->movies.end(); ++itr)
	{
		tempSubtotal += (*itr)->getPrice();
	}


	this->setSubtotal(tempSubtotal);
	return tempSubtotal;
}



void Cart::displayCart()
{
	vector<Movie*>::iterator itr;
	vector<Movie*> movieList = this->movies;
	std::map<int, Movie*> allMovies;
	
	if (movieList.empty())
	{
		cout << "---> Your Cart is Empty <---" << endl;
		return;
	}

	int counter = 1;
	cout << "---> Your Cart <---" << endl;

	for (itr = movieList.begin(); itr != movieList.end(); itr++)
	{
		allMovies[counter] = *itr;
		cout << setw(1) << left << counter << ". " << setw(14) << left << (*itr)->getTitle() << left << "(x" << (*itr)->getQuantity() << ")";;
		cout << "$" << fixed << setprecision(2) << (*itr)->getPrice() << endl;
		counter++;
	}
	cout  << "-------------------" << endl << endl;

}


void Cart::displayAddToCart()
{
	vector<Movie*>::iterator itr;
	vector<Movie*> movieList = this->movies;
	std::map<int, Movie*> allMovies;

	if (movieList.empty())
	{
		cout << "---> Your Cart is Empty <---" << endl;
		return;
	}

	int counter = 1;
	cout << "---> Your Cart <---" << endl;

	for (itr = movieList.begin(); itr != movieList.end(); itr++)
	{
		allMovies[counter] = *itr;
		cout << setw(1) << left << counter << ". " << setw(14) << left << (*itr)->getTitle() << left << "(x" << (*itr)->getQuantity() << ")";;
		cout << "$" << fixed << setprecision(2) << (*itr)->getPrice() << endl;
		counter++;
	}
	cout << "-------------------" << endl << endl;


	bool doneSelecting = false;
	string selection = "";
	int selectionInt = 0;

	while (!doneSelecting)
	{
		cout << "Which item would you like to add to your cart?" << endl;
		cout << "Type 'DONE' to stop." << endl;
		cout << "# of Movie: ";
		cin >> selection;
		if (selection == "DONE")
		{
			cout << endl;
			break;
		}
		selectionInt = stoi(selection);
		if (selectionInt < 0 || selectionInt > counter)
		{
			cout << "That was not a valid movie. Please select a movie from the list." << endl << endl;
			selection = "";
			selectionInt = 0;
			continue;
		}
		Movie* selectedMovie = allMovies[selectionInt];
		Cart* accountCart = this->account->getCart();
		accountCart->addItem(selectedMovie);

		cout << endl << "\"" << selectedMovie->getTitle() << "\" was added to your cart." << endl << endl;

		if (movieList.empty())
		{
			cout << "---> Your Cart is Empty <---" << endl;
			return;
		}

		int counter = 1;
		cout << "---> Your Cart <---" << endl;

		for (itr = movieList.begin(); itr != movieList.end(); itr++)
		{
			allMovies[counter] = *itr;
			cout << setw(1) << left << counter << ". " << setw(14) << left << (*itr)->getTitle() << left << "(x" << (*itr)->getQuantity() << ")";;
			cout << "$" << fixed << setprecision(2) << (*itr)->getPrice() << endl;
			counter++;
		}
		cout << "-------------------" << endl << endl;


	}

}


void Cart::displayRemoveFromCart()
{
	vector<Movie*>::iterator itr;
	vector<Movie*> movieList = this->movies;
	std::map<int, Movie*> allMovies;

	if (movieList.empty())
	{
		cout << "---> Your Cart is Empty <---" << endl;
		return;
	}

	int counter = 1;
	cout << "---> Your Cart <---" << endl;

	for (itr = movieList.begin(); itr != movieList.end(); itr++)
	{
		allMovies[counter] = *itr;
		cout << setw(1) << left << counter << ". " << setw(14) << left << (*itr)->getTitle() << left << "(x" << (*itr)->getQuantity() << ")";;
		cout << "$" << fixed << setprecision(2) << (*itr)->getPrice() << endl;
		counter++;
	}
	cout << "-------------------" << endl << endl;


	bool doneSelecting = false;
	string selection = "";
	int selectionInt = 0;

	while (!doneSelecting)
	{
		cout << "Which item would you like to remove from your cart?" << endl;
		cout << "Type 'DONE' to stop." << endl;
		cout << "# of Movie: ";
		cin >> selection;
		if (selection == "DONE")
		{
			cout << endl;
			break;
		}
		selectionInt = stoi(selection);
		if (selectionInt < 0 || selectionInt > counter)
		{
			cout << "That was not a valid movie. Please select a movie from the list." << endl << endl;
			selection = "";
			selectionInt = 0;
			continue;
		}
		Movie* selectedMovie = allMovies[selectionInt];
		Cart* accountCart = this->account->getCart();
		accountCart->decItem(selectedMovie);

		cout << endl << "\"" << selectedMovie->getTitle() << "\" was removed from your cart." << endl << endl;

		if (movieList.empty())
		{
			cout << "---> Your Cart is Empty <---" << endl;
			return;
		}

		int counter = 1;
		cout << "---> Your Cart <---" << endl;

		for (itr = movieList.begin(); itr != movieList.end(); itr++)
		{
			allMovies[counter] = *itr;
			cout << setw(1) << left << counter << ". " << setw(14) << left << (*itr)->getTitle() << left << "(x" << (*itr)->getQuantity() << ")";;
			cout << "$" << fixed << setprecision(2) << (*itr)->getPrice() << endl;
			counter++;
		}
		cout << "-------------------" << endl << endl;


	}

}




