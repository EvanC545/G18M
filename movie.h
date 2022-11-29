#pragma once
#include <string>

using std::string;

class Account;

class Movie {

private:

	int id;
	string title;
	string genre;
	int released;
	double length;
	string director;
	int reviews;
	double price;


public:

	//constructors
	Movie();
	Movie(string p_title, string gen, int rel, double len, string dir, int rev, double p_price);

	//getters
	int getID();
	string getTitle();
	string getGenre();
	int getReleased();
	double getLength();
	string getDirector();
	int getReviews();
	double getPrice();

	//setters
	void setTitle(string p_title);
	void setGenre(string gen);
	void setReleased(int rel);
	void setLength(double len);
	void setDirector(string dir);
	void setReviews(int rev);
	void setPrice(double p_price);
	void setID(int id);

};


class MovieManager {
public:
	void displayAndChooseMovies(Account* account);
	bool addMovieToCart();
};