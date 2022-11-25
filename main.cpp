#include <iostream>

using namespace std;
class Account {
private:
	string name;
	string email;
	string password;
	string shippingName;
	string shippingAddressLine1;
	string shippingAddressLine2;
	string shippingCity;
	string shippingState;
	string shippingZip;
	string cardHolder;
	string cardNumber;
	string cardExpDate;
	string cardSecurityCode;
	int currentCardId;
public:
	void setName(string name);
	string getName();
	void setEmail(string email);
	string getEmail();
	void setPassword(string oldPassword, string newPassword);
	bool authenticate(string email, string password);
	void setShippingAddress(string address);
	string getShippingAddress();
	void setBillingAddress(string address);
	string getBillingAddress();
	void setCard(string currentCardId);
	// Datatype?--> string getCard();
	void setCart(string cartId);
	// Datatype? --> string getCart();
};

class Cart {
private:
	string item;
	double itemPrice;
	int numItems;
	double totalCost;
public:
	string viewCart(string item);
	bool cartIsEmpty();
	void addItem(string item);
	void decItem(string item, bool cartIsEmpty);
	void removeItem(string item);
	double calcSubtotal(double itemPrice);
	void checkout(string item, double calcSubtotal);
};

//globals
bool loggedin = false;
int menu_items;

//function to display menu based upon being logged in or not
void displayMenu() {

	if (!loggedin) {

		cout << "1. Login\n";
		cout << "2. Create Account\n";
		cout << "3. Exit Program\n\n";
		menu_items = 3;

	}
	else {

		cout << "1. View Items\n";
		cout << "2. Cart Info\n";
		cout << "3. Checkout\n";
		cout << "4. Order History\n";
		cout << "5. Edit Account\n";
		cout << "6. Exit Program\n\n";
		menu_items = 6;

	}

}

int main() {
	
	//display menu
	displayMenu();
	
	//get user choice
	int choice;

	do {

		cout << ">> ";
		cin >> choice;

	} while (choice < 1 || choice > menu_items);
	
	//perform user desired action
	if (!loggedin) {

		switch (choice) {

		case 1: 
			//login
			//set loggedin to true
			break;
		case 2:
			//create account
			break;
		case 3:
			exit(0);
			break;

		}

	}
	else {

		switch (choice) {

		case 1:
			//view items
			break;
		case 2:
			//cart info
			break;
		case 3:
			//checkout
			break;
		case 4:
			//order history
			break;
		case 5:
			//edit account
			break;
		case 6:
			exit(0);
			break;

		}

	}

	return 0;
}
