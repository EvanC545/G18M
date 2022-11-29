#include <iostream>
#include <sqlite3.h>
#include <string>
#include <iterator>
#include <map>
#include "movie.h"
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

static int busy_handler(void* data, int attempt) {
	printf("attempt: %d\n", attempt);
	if (attempt < 50) {
		sqlite3_sleep(500);
		return 1;
	}
	return 0;
}


//constructors definition
Movie::Movie() {};

Movie::Movie(string p_title, string gen, int rel, double len, string dir, int rev, double p_price) {

	title = p_title;
	genre = gen;
	released = rel;
	length = len;
	director = dir;
	reviews = rev;
	price = p_price;

};

//getters definitions
string Movie::getTitle() {

	return title;

};

string Movie::getGenre() {

	return genre;

};

int Movie::getReleased() {

	return released;

};

float Movie::getLength() {

	return length;

};

string Movie::getDirector() {

	return director;

};

int Movie::getReviews() {

	return reviews;

};

float Movie::getPrice() {

	return this->price * this->quantity;
};

//setters definitions
void Movie::setTitle(string p_title) {

	title = p_title;

};
void Movie::setGenre(string gen) {

	genre = gen;

};

void Movie::setReleased(int rel) {

	released = rel;

};

void Movie::setLength(double len) {

	length = len;

};

void Movie::setDirector(string dir) {

	director = dir;

};

void Movie::setReviews(int rev) {

	reviews = rev;

};

void Movie::setPrice(float p_price) {

	price = p_price;

};

void Movie::setID(int id) {

	this->id = id;
};

int Movie::getID() {

	return this->id;
};

int Movie::getQuantity() {

	return this->quantity;
};

int Movie::incrementQuantity()
{
	int newQuantity = this->quantity + 1;
	this->quantity = newQuantity;
	return newQuantity;
}


int Movie::decrementQuantity()
{
	int newQuantity = this->quantity - 1;
	this->quantity = newQuantity;
	return newQuantity;
}

void Movie::setQuantity(int quantity)
{
	this->quantity = quantity;
}

void MovieManager::displayAndChooseMovies(Account* account)
{
	// Show movies
	// Maybe use map to store ID with corresponding list number
	sqlite3* db = nullptr;
	sqlite3_stmt* stmt = nullptr;
	char* zErrMsg = 0;
	int rc;
	db = getDBConnection(DB_NAME, db);
	std::map<int, Movie*> allMovies;

	string movieSelectSql = "SELECT * FROM Movies;";
	rc = sqlite3_prepare(db, movieSelectSql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL Statement error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	rc = sqlite3_step(stmt);

	int counter = 1;
	while (sqlite3_column_text(stmt, 1))
	{
		Movie* tempMovie = new Movie();

		char* movieIDChar = (char*)sqlite3_column_text(stmt, 1);
		int movieID = strtol(movieIDChar, NULL, 10);
		tempMovie->setID(movieID);
		char* movieTitle = (char*)sqlite3_column_text(stmt, 0);
		tempMovie->setTitle(movieTitle);
		char* movieGenre = (char*)sqlite3_column_text(stmt, 2);
		if (movieGenre)
		{
			tempMovie->setGenre(movieGenre);
		}
		char* movieReleasedChar = (char*)sqlite3_column_text(stmt, 3);
		if (movieReleasedChar)
		{
			int movieReleased = strtol(movieReleasedChar, NULL, 10);
			tempMovie->setReleased(movieReleased);
		}
		
		char* moviePriceChar = (char*)sqlite3_column_text(stmt, 5);
		if (moviePriceChar)
		{
			float moviePrice = strtof(moviePriceChar, NULL);
			tempMovie->setPrice(moviePrice);
		}



		allMovies.insert(std::pair<int, Movie*>(counter, tempMovie));
		rc = sqlite3_step(stmt);

		counter++;
	}
	// USED RESET TO STOP BUSY
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);


	cout << "---> Available Movies <---" << endl;
	map<int, Movie*>::iterator itr;

	for (itr = allMovies.begin(); itr != allMovies.end(); itr++)
	{
		cout << itr->first << ": " << itr->second->getTitle() << endl;
	}

	bool doneSelecting = false;
	string selection = "";
	int selectionInt = 0;

	while (!doneSelecting)
	{
		cout << endl << "Type # of Movie to add it to your cart: " << endl;
		cout << "Type 'DONE' to stop choosing movies" << endl;
		cout << "# of Movie: ";
		cin >> selection;
		if (selection == "DONE")
		{
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
		Cart* accountCart = account->getCart();

		accountCart->addItem(selectedMovie);

		cout << endl << "\"" << selectedMovie->getTitle() << "\" was added to your cart." << endl;

		sqlite3_close(db);

	}

}

