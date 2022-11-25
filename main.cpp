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
int choice;

//function to get user choice for menu
void getChoice() {

	do {

		cout << ">> ";
		cin >> choice;

	} while (choice < 1 || choice > menu_items);

}

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

//function to display nested menus for cart info, checkout, order history, and edit account
//each will be represented by numbers 1 through 4 respectively
void displayNested(int nest) {

	switch (nest) {

	case 1:
		cout << endl;
		cout << "1. Go Back\n";
		cout << "2. View Cart\n";
		cout << "3. Remove Item\n";
		cout << "4. Add Item\n\n";
		menu_items = 4;
		break;
	case 2:
		cout << endl;
		cout << "1. View Cart\n";
		cout << "2. Input Shipping/Billing Address\n";
		cout << "3. Input Card Info\n";
		cout << "4. Confirm Checkout\n\n";
		menu_items = 4;
		break;
	case 3:
		cout << endl;
		cout << "1. View Past Orders\n\n";
		menu_items = 1;
		break;
	case 4:
		cout << endl;
		cout << "1. Edit Addresses\n";
		cout << "2. Edit Card\n";
		cout << "3. Edit Name/Email\n";
		cout << "4. Change Password\n";
		cout << "5. Delete Account\n\n";
		menu_items = 5;
		break;

	}

}

int main() {
	
	//display menu
	displayMenu();
	
	//get user choice
	getChoice();
	
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
			displayNested(1);
			getChoice();
			break;
		case 3:
			//checkout
			displayNested(2);
			getChoice();
			break;
		case 4:
			//order history
			displayNested(3);
			getChoice();
			break;
		case 5:
			//edit account
			displayNested(4);
			getChoice();
			break;
		case 6:
			exit(0);
			break;

		}

	}

	return 0;
}
