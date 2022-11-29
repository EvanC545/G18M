#include <iostream>
#include <sqlite3.h>
#include <string>
#include <iterator>
#include <map>
#include "movie.h"

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

double Movie::getLength() {

	return length;

};

string Movie::getDirector() {

	return director;

};

int Movie::getReviews() {

	return reviews;

};

double Movie::getPrice() {

	return price;

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

void Movie::setPrice(double p_price) {

	price = p_price;

};

void Movie::setID(int id) {

	this->id = id;
};



void MovieManager::displayAllMovies()
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
		
		char* moviePriceChar = (char*)sqlite3_column_text(stmt, 3);
		if (moviePriceChar)
		{
			int moviePrice = strtof(movieReleasedChar, NULL);
			tempMovie->setPrice(moviePrice);
		}

		allMovies.insert(std::pair<int, Movie*>(counter, tempMovie));
		rc = sqlite3_step(stmt);
		counter++;
	}

	cout << "TOTAL MAP:" << endl;
	map<int, Movie*>::iterator itr;

	for (itr = allMovies.begin(); itr != allMovies.end(); itr++)
	{
		cout << itr->first << ": " << itr->second->getTitle() << endl;
	}
	


}