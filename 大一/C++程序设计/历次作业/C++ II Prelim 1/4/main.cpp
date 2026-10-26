#include <bits/stdc++.h>
#include "Emploee.h"
using namespace std;

//Prototype
double calculateEarningsWithBonus(const Employee* emp, int currentMonth);
void virtualViaPointer(const Employee* emp, int currentMonth);

int main() {
    //Ciallo～ (∠・ω< )⌒★
    cout << fixed << setprecision(2);

    int currentMonth;
    cout << "Please type  current month\n";
    cin >> currentMonth;

    //Create Employee
    Date birth1(5, 15, 1990);
    SalariedEmployee emp1("San", "Zhang", "111-11-1111", birth1, 800.0);

    Date birth2(3, 20, 1985);
    SalariedEmployee emp2("Si", "Li", "333-33-3333", birth2, 700.0);

    Date birth3(5, 8, 1988);
    SalariedEmployee emp3("Wu", "Wang", "444-44-4444", birth3, 900.0);

    Date birth4(12, 1, 1995);
    SalariedEmployee emp4("Liu", "Zhao", "555-55-5555", birth4, 750.0);

    //Create vector to store
    vector<Employee*> employees;
    employees.push_back(&emp1);
    employees.push_back(&emp2);
    employees.push_back(&emp3);
    employees.push_back(&emp4);

    for (Employee* emp : employees) {
        double salary = emp->earnings();
    
        //Birthday?
        if (emp->isBirthdayThisMonth(currentMonth)) {
            salary += 100.0;
            cout << "Birthday Bounus! \n";
        }

        emp->print();
        cout << "\n\n";
    }
    return 0;
}
double calculateEarningsWithBonus(const Employee* emp, int currentMonth) {
    double baseEarnings = emp->earnings();
    if (emp->isBirthdayThisMonth(currentMonth)) {
        return baseEarnings + 100.0;
    }
    return baseEarnings;
}

void virtualViaPointer(const Employee* emp, int currentMonth) {
    emp->print();
    cout << "\nEarning This Month: $" << calculateEarningsWithBonus(emp, currentMonth) << "\n\n";
}
