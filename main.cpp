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

int main() {

	return 0;
}