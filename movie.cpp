#include <iostream>
#include "movie.h"

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