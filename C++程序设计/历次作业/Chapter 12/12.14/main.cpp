#include "Account.h"
using namespace std;

double getAmount(const string& prompt) {
    double amount;
    cout << prompt;
    cin >> amount;
    return amount;
}

void processAccount(Account* account, int accountNum) {
    cout << "\nProcessing " << account->getAccountType() << " #" << accountNum << endl;
    cout << "\nCurrent balance: $" << account->getBalance() << endl;

    cout << "\nWithdrawal\n";
    double withdrawAmount = getAmount("Enter amount to withdraw: $");

    if (account->debit(withdrawAmount)) {
        cout << "Withdrawal successful!" << endl;
    }
    else {
        cout << "Withdrawal failed!" << endl;
    }
    cout << "Balance after withdrawal: $" << account->getBalance() << endl;

    cout << "\nDeposit\n";
    double depositAmount = getAmount("Enter amount to deposit: $");

    account->credit(depositAmount);
    cout << "Balance after deposit: $" << account->getBalance() << endl;

    if (SavingsAccount* savingsPtr = dynamic_cast<SavingsAccount*>(account)) {
        cout << "\nInterest Calculation\n";
        double interest = savingsPtr->calculateInterest();
        cout << "Interest rate: " << savingsPtr->getInterestRate() << "%" << endl;
        cout << "Interest earned: $" << interest << endl;
        savingsPtr->credit(interest);
        cout << "Interest added to account!" << endl;
    }

    cout << "\nFinal Balance\n" << endl;
    cout << "Updated balance: $" << account->getBalance() << endl;
}

int main() {
    // Set output format
    cout << fixed << setprecision(2);

    // Create account objects
    SavingsAccount savings1(1000.00, 5.0);
    SavingsAccount savings2(2000.00, 3.5);
    CheckingAccount checking1(500.00, 1.50);
    CheckingAccount checking2(800.00, 2.00);

    vector<Account*> accounts;
    accounts.push_back(&savings1);
    accounts.push_back(&savings2);
    accounts.push_back(&checking1);
    accounts.push_back(&checking2);

    for (size_t i = 0; i < accounts.size(); i++) {
        cout << "Account " << (i + 1) << ": " << accounts[i]->getAccountType()
            << " Balance: $" << accounts[i]->getBalance() << endl;
    }

    // Process each account in the vector
    for (size_t i = 0; i < accounts.size(); i++) {
        processAccount(accounts[i], i + 1);
    }

    // Display final summary of all accounts
    double totalBalance = 0.0;
    for (size_t i = 0; i < accounts.size(); i++) {
        cout << "Account " << (i + 1) << " (" << accounts[i]->getAccountType() << "): $"
            << accounts[i]->getBalance() << endl;
        totalBalance += accounts[i]->getBalance();
    }

    cout << "TOTAL BALANCE OF ALL ACCOUNTS: $" << totalBalance << endl;
    return 0;
}