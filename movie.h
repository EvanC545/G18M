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
	float length;
	string director;
	int reviews = 0;
	float price = 0.00;
	int quantity = 0;

public:

	//constructors
	Movie();
	Movie(string p_title, string gen, int rel, double len, string dir, int rev, double p_price);

	//getters
	int getID();
	string getTitle();
	string getGenre();
	int getReleased();
	float getLength();
	string getDirector();
	int getReviews();
	float getPrice();

	//setters
	void setTitle(string p_title);
	void setGenre(string gen);
	void setReleased(int rel);
	void setLength(double len);
	void setDirector(string dir);
	void setReviews(int rev);
	void setPrice(float p_price);
	void setID(int id);

	int incrementQuantity();
	int decrementQuantity();
	void setQuantity(int quantity);
	int getQuantity();
	float getBasePrice();

};


class MovieManager {
public:
	void displayAndChooseMovies(Account* account);

	bool addMovieToCart();
};