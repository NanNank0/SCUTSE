#include "Account.h"
using namespace std;

//Account Base Class
//Constructor
Account::Account(double Balance) {
	balance = Balance;
}

void Account::credit(double amount) {
	balance += amount;
}
bool Account::debit(double amount) {
	if (amount > 0 && amount <= balance) {
		balance -= amount;
		return true;
	}
	return false;
}
double Account::getBalance() const {
	return balance;
}

//Savings Account Class
//Constructor
SavingsAccount::SavingsAccount(double Balance, double rate) :Account(Balance){
	interestRate = rate;
}
double SavingsAccount::calculateInterest() const {
	return balance * interestRate / 100;
}

//Checking Account Calss
//Constructor
CheckingAccount::CheckingAccount(double Balance, double fee) : Account(Balance) {
	transactionFee = fee;
}
bool CheckingAccount::debit(double amount) {
	bool success = Account::debit(amount);
	if (success) {
		balance -= transactionFee;
	}
	return success;
}
void CheckingAccount::credit(double amount) {
	Account::credit(amount);
	if (amount > 0) {
		balance -= transactionFee;
	}
}