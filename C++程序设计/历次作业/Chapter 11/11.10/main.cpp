#include <bits/stdc++.h>
#include "Account.h"
using namespace std;
int main() {
	//Ciallo～ (∠・ω< )⌒★
	cout << fixed << setprecision(2);

	//Test for Base Account
	cout << "Base Account\n";
	Account account1(500.00);
	cout << "Credit Test, 100$\n";
	account1.credit(100);
	cout << "New balance: " << account1.getBalance() << endl;
	cout << "Debit Test, 100$\n";
	account1.debit(100);
	cout << "New balance: " << account1.getBalance() << endl;

	//Test for Savings Account
	cout << "\nSavings Account\n";
	SavingsAccount account2(500, 5);
	cout << "Credit Test, 100$\n";
	account2.credit(100);
	cout << "New balance: " << account2.getBalance() << endl;
	cout << "Debit Test, 100$\n";
	account2.debit(100);
	cout << "New balance: " << account2.getBalance() << endl;
	cout << "Calculate and add interest test:\n";
	double interest = account2.calculateInterest();
	account2.credit(interest);
	cout << "Interest earn: " << interest << endl;
	cout << "New balance: " << account2.getBalance() << endl;

	//Test for Checking Account
	cout << "\nChecking Account\n";
	CheckingAccount account3(500,2);
	cout << "Credit Test, 100$\n";
	account3.credit(100);
	cout << "New balance: " << account3.getBalance() << endl;
	cout << "Debit Test, 100$\n";
	account3.debit(100);
	cout << "New balance: " << account3.getBalance() << endl;

	return 0;
}