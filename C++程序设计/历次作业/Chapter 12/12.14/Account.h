#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <bits/stdc++.h>
using namespace std;

//Base Class Account
class Account {
protected:
    double balance;

public:
    // Constructor with validation
    Account(double initialBalance);
    virtual ~Account() = default;

    virtual void credit(double amount);
    virtual bool debit(double amount);
    double getBalance() const;
    virtual string getAccountType() const;
};

//Savings Account
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(double initialBalance, double rate);
    double calculateInterest() const;
    string getAccountType() const override;
    double getInterestRate() const;
};

//Checking Account
class CheckingAccount : public Account {
private:
    double transactionFee;

public:
    CheckingAccount(double initialBalance, double fee);
    void credit(double amount) override;
    bool debit(double amount) override;
    string getAccountType() const override;
};

#endif // ACCOUNT_H