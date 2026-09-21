#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <bits/stdc++.h>
using namespace std;

class Account {
protected:
	double balance;
public:
	//Constructor
	Account(double Balance);
	//Virtual destructor
	virtual ~Account() = default;

	virtual void credit(double amount);

	virtual bool debit(double amount);

	double getBalance() const;
};

class SavingsAccount : public Account {
private:
	double interestRate;
public:
	//Constructor
	SavingsAccount(double Banlance, double rate);

	double calculateInterest() const;
};

class CheckingAccount : public Account {
private:
	double transactionFee;
public:
	//Constructor
	CheckingAccount(double Balance, double fee);
	void credit(double amount) override;
	bool debit(double amount) override;
};
#endif // !ACCOUNT_H
