#include <iostream>
#include <sqlite3.h>
#include "account.h"
#include "movie.h"
#include "order.h"
#include "cart.h"

using namespace std;

string DB_NAME = "G18M.sqlite";

//globals

//CHANGE TO FALSE
bool loggedin = false;
bool nested = false;
int menu_items;
int choice;
Account* account;
Address* shippingAddress;
Address* billingAddress;


//function to get user choice for menu
void getChoice() {

	do {

		cout << ">> ";
		cin >> choice;

	} while (choice < 1 || choice > menu_items);

}

//function to display menu based upon being logged in or not
void displayMenu() {

	nested = false;

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
		cout << "6. Logout\n";
		cout << "7. Exit Program\n\n";
		menu_items = 7;

	}

}

//function to display nested menus for cart info, checkout, order history, and edit account
//each will be represented by numbers 1 through 4 respectively
void displayNested(int nest) {

	nested = true;

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
		cout << "1. Go Back\n";
		cout << "2. View Cart\n";
		cout << "3. Checkout\n\n";
		
		menu_items = 3;
		break;
	case 3:
		cout << endl;
		cout << "1. Go Back\n";
		cout << "2. View Past Orders\n\n";
		menu_items = 2;
		break;
	case 4:
		cout << endl;
		cout << "1. Go Back\n";
		cout << "2. Edit Addresses\n";
		cout << "3. Edit Card\n";
		cout << "4. Edit Name/Email\n";
		cout << "5. Change Password\n";
		cout << "6. Delete Account\n\n";
		menu_items = 6;
		break;

	}

}

//function to handle user desired action for cart info, checkout, order history, and edit account nested menus
void nestedAction(int nest) {
	switch (nest) {

	case 1:
	{
		//cart info nested menu branch
		switch (choice) {

			case 1:
				//go back
				break;
			case 2:
			{
				//view cart
				Cart* accountCart = account->getCart();
				accountCart->displayCart();
				break;
			}
			case 3:
			{
				//remove item
				Cart* accountCart = account->getCart();
				accountCart->displayRemoveFromCart();
				break;
			}
			case 4:
			{
				//add item
				Cart* accountCart = account->getCart();
				accountCart->displayAddToCart();
				break;
			}
		}
		break;
	}
	case 2:

		//checkout nested menu branch
		switch (choice) {

		
		case 1:
			//go back
			break;
		case 2:
		{
			//view cart
			Cart * accountCart = account->getCart();
			accountCart->displayCart();
			break;
		}
		case 3:
			//confirm checkout
			Cart * accountCart = account->getCart();
			if (accountCart->cartIsEmpty())
			{
				cout << endl << "Your Cart is Empty. Please add an item before checking out." << endl << endl;
				break;
			}

			if (account->hasValidAddresses())
			{
				string input = "";
				bool confirmedOrder = false;

				while (!confirmedOrder)
				{
					cout << endl << "Use Default Addresses? (y/n): ";
					cin >> input;
					if (input[0] == 'y')
					{
						break;
					}
				}
			}
			else
			{
				account->displayEditAddressInfo();
			}
			account->displayEditCardInfo();
			account->displayConfirm();

			break;

		}
		break;

	case 3:

		//order history nested menu branch
		switch (choice) {

		case 1:
			//go back
			break;
		case 2:
			//view past orders
			account->displayPastOrders();
			break;
		}
		break;

	case 4:

		//edit account nested menu branch
		switch (choice) {

		case 1:
			//go back
			break;
		case 2:
			//edit addresses
			account->displayEditAddressInfo();
			cout << endl;
			break;
		case 3:
			//edit card
			account->displayEditCardInfo();
			cout << endl;

			break;
		case 4:
			//edit name/email
			account->displayEditNameEmail();
			cout << endl;

			break;
		case 5:
			//change password
			account->displayEditPassword();
			cout << endl;
			break;
		case 6:
			//delete account
			bool deleted = account->deleteAccount();
			if (deleted)
			{
				loggedin = false;
			}
			cout << endl;
			break;

		}
		break;

	}

}

int main() {

	account = nullptr;
	do {

		//display menu
		displayMenu();

		//get user choice
		getChoice();



		//perform user desired action
		if (!loggedin) {

			string name;
			string email;
			string password1;
			string password2;
			bool validPasswords = false;
			int idOrFalse = 0;
			AccountManager accountManager;
			switch (choice) {

			case 1:
			{
				//login
				//set loggedin to true
				
				while (!idOrFalse)
				{
					cout << endl << "Please enter your email: ";
					cin >> email;
					cout << endl << "Please enter your password: ";
					cin >> password1;
					idOrFalse = accountManager.authenticate(email, password1);
				}
				account = new Account(idOrFalse);
				if (account)
				{
					cout << endl << "You are now logged in as " << account->getName() << endl << endl;
					loggedin = true;
					Cart* cart = new Cart(account);
					account->setCart(cart);
				}

				break;
			}
			case 2:
			{
				//create account

				cout << "Please enter your name: ";
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				getline(cin, name);
				cout << endl << "Please enter your email: ";
				cin >> email;
				while (!validPasswords)
				{
					cout << endl << "Please enter your password: ";
					cin >> password1;
					cout << endl << "Please enter your password again: ";
					cin >> password2;
					if (password1 != password2)
					{
						cout << "Passwords don't match. Please try again.";
					}
					else
					{
						validPasswords = true;
					}
				}
				account = new Account(name, email, password1);

				cout << endl << "You are now logged in as " << account->getName() << endl << endl;

				loggedin = true;
				Cart* cart = new Cart(account);
				account->setCart(cart);

				break;
			}
			case 3:
				exit(0);
				break;

			}

		}
		else {

			switch (choice) {

			case 1:
				//view items
				MovieManager movieManager;
				movieManager.displayAndChooseMovies(account);
				break;
			case 2:
				//cart info
				displayNested(1);
				getChoice();
				nestedAction(1);
				break;
			case 3:
				//checkout
				displayNested(2);
				getChoice();
				nestedAction(2);
				break;
			case 4:
				//order history
				displayNested(3);
				getChoice();
				nestedAction(3);
				break;
			case 5:
				//edit account
				displayNested(4);
				getChoice();
				nestedAction(4);
				break;
			case 6:
				//logout
				//set loggedin to false
				loggedin = false;
				break;
			case 7:
				exit(0);
				break;

			}

		}

	} while (choice != menu_items || nested);
	
	

	return 0;
}
