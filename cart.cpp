#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include "cart.h"

using namespace std;
//constructors definitions

Cart::Cart() {};

// Cart::Cart(string c_item, double price) {
  
//   tempItem = "";
//   item.push_back(c_item);
//   itemPrice.push_back(price);
//   numItems = item.size();
//   totalCost = 0;
 
// };

// Function definitions
//void Cart::viewCart(vector<string> item, vector<double> itemPrice) {
//  for (int i = 0; i < item.size(); i++) {
//     cout << i+1 << ")" << item[i] << ": $" << itemPrice[i] << endl;  
//  }
//  // TODO: Ensure this works properly
//};
void Cart::viewCart() {
    vector<Movie>::iterator ptr;
    int counter = 1;
    for (ptr = items.begin(); ptr < items.end(); ptr++)
    {
        cout << counter << ")" << ptr->getTitle() << ": $" << ptr->getPrice() << endl;
    }
};

//bool Cart::cartIsEmpty(vector<string> item) {
//  return item.empty();
//};
bool Cart::cartIsEmpty() {
    if (items.size() < 1)
    {
        return true;
    }
    return false;
};

void Cart::addItem(vector<string> item, vector<double> itemPrice, string tempItem) {
    bool itemAdded = false;
    double tempItemPrice = 0.0;
    for (int i = 0; i < item.size(); i++) {
      if (item[i] == tempItem) {
        item.push_back(tempItem);
        tempItemPrice = itemPrice[i];
        itemPrice.push_back(tempItemPrice);
        itemAdded = true;
      }
    }
    if (itemAdded) {
      cout << "Item added!" << endl;
    }
};
void Cart::addItem(Movie item) {
    items.push_back(item)
};

void Cart::decItem(vector<string> item, vector<double> itemPrice, string tempItem) {
  bool itemRemoved = false;
  if (cartIsEmpty() || item.empty()) {
    cout << "The cart is empty!" << endl;
  }
  else {
    for (int i = 0; i < item.size(); i++) {
      if (item[i] == tempItem) {
        item.erase(i);
        itemPrice.erase(i);
        itemRemoved = true;
      }
    }
  }
  if (itemRemoved) {
    cout << "Item removed!" << endl;
  }
};
void Cart::decItem(string tempItem) {
  decItem(vector<string> item, vector<double> itemPrice, string tempItem);
};

void Cart::removeItem(vector<string> item, vector<double> itemPrice, string tempItem) {
  bool itemRemoved = false;
  if (cartIsEmpty() || item.empty()) {
    cout << "The cart is empty!" << endl;
  }
  else {
    for (int i = 0; i < item.size(); i++) {
      if (item[i] == tempItem) {
        item.erase(i);
        itemPrice.erase(i);
        itemRemoved = true;
      }
    }
  }
  if (itemRemoved) {
    cout << "Item removed!" << endl;
  }
};

void Cart::removeItem(string tempItem) {
  removeItem(vector<string> item, vector<double> itemPrice, string tempItem);
};

void Cart::calcSubtotal(vector<double> itemPrice) {
  double subTotal = 0.0;
  if (itemPrice.empty()) {
    cout << "Subtotal is $0.00" << endl;
  }
  else {
    for (int i = 0; i < itemPrice.size() {
      subTotal += itemPrice[i];
    }
      cout << "The subtotal is $" << subTotal << endl;   
   }
};
void Cart::calcSubtotal() {
  calcSubtotal(vector<double> itemPrice);
};

void Cart::checkout(vector<string> item, vector<double> itemPrice) {
  double subTotal = 0.0;
  if (item.empty() != true) {
    for (int i = 0; i < item.size(); i++) {
      cout << i+1 << ")" << item[i] << ": $" << itemPrice[i] << endl;
      subTotal += itemPrice[i];
    }
    subTotal *= 1.07;
    cout << "Your order total is $" << subTotal << "." << endl;
  }
  else {
    cout << "Your cart is empty!" << endl;
  }
};
void Cart::checkout() {
  checkout(vector<string> item, vector<double> itemPrice);
};










