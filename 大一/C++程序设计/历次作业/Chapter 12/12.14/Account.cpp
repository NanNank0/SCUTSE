#include "Account.h"
using namespace std;

//Account Base Class
Account::Account(double initialBalance) {
    if (initialBalance >= 0.0) {
        balance = initialBalance;
    }
    else {
        balance = 0.0;
        cout << "Error: Initial balance was invalid. Setting balance to 0.0." << endl;
    }
}

void Account::credit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
    else {
        cout << "Error: Credit amount must be positive." << endl;
    }
}

bool Account::debit(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        return true;
    }

    if (amount > balance) {
        cout << "Debit amount exceeded account balance." << endl;
    }
    else if (amount <= 0) {
        cout << "Debit amount must be positive." << endl;
    }
    return false;
}

double Account::getBalance() const {
    return balance;
}

string Account::getAccountType() const {
    return "Account";
}

//Savings Account
SavingsAccount::SavingsAccount(double initialBalance, double rate)
    : Account(initialBalance) {
    if (rate >= 0.0) {
        interestRate = rate;
    }
    else {
        interestRate = 0.0;
        cout << "Error: Interest rate was invalid. Setting to 0.0." << endl;
    }
}

double SavingsAccount::calculateInterest() const {
    // Interest = balance * (interestRate / 100)
    return balance * interestRate / 100.0;
}

string SavingsAccount::getAccountType() const {
    return "SavingsAccount";
}

double SavingsAccount::getInterestRate() const {
    return interestRate;
}

//Checking Account
CheckingAccount::CheckingAccount(double initialBalance, double fee)
    : Account(initialBalance) {
    if (fee >= 0.0) {
        transactionFee = fee;
    }
    else {
        transactionFee = 0.0;
        cout << "Error: Transaction fee was invalid. Setting to 0.0." << endl;
    }
}

void CheckingAccount::credit(double amount) {
    Account::credit(amount);
    if (amount > 0) {
        balance -= transactionFee;
        cout << "  Transaction fee $" << transactionFee << " charged" << endl;
    }
}

bool CheckingAccount::debit(double amount) {
    bool success = Account::debit(amount);
    if (success) {
        balance -= transactionFee;
        cout << "  Transaction fee $" << transactionFee << " charged" << endl;
    }

    return success;
}

string CheckingAccount::getAccountType() const {
    return "CheckingAccount";
}