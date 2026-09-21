#include "Employee.h"
using namespace std;
//prototype
void virtualViaPointer(const Employee*, int);

int main() {

    cout << fixed << setprecision(2);
    int currentMonth;
    cin >> currentMonth;

    cout << "Current Month: " << currentMonth << endl;

    Date birthday1(1990, 5, 15); 
    Date birthday2(1985, 10, 20);
    Date birthday3(1992, 5, 8);  
    Date birthday4(1988, currentMonth, 3);

    SalariedEmployee salariedEmployee("John", "Smith", "111-11-1111", birthday1, 800.00);
    CommissionEmployee commissionEmployee("Sue", "Jones", "333-33-3333", birthday2, 10000.00, 0.06);
    BasePlusCommissionEmployee basePlusCommissionEmployee("Bob", "Lewis", "444-44-4444", birthday3, 5000.00, 0.04, 300.00);
    SalariedEmployee birthdayEmployee("Alice", "Wonder", "555-55-5555", birthday4, 750.00);

    vector<Employee*> employees;
    employees.push_back(&salariedEmployee);
    employees.push_back(&commissionEmployee);
    employees.push_back(&basePlusCommissionEmployee);
    employees.push_back(&birthdayEmployee);


    // Process each employee polymorphically
    for (const Employee* employeePtr : employees) {
        virtualViaPointer(employeePtr, currentMonth);
    }

    cout << "========================================" << endl;
    cout << "Payroll processing complete!" << endl;
    cout << "(Birthday bonus: $100.00 added if birthday in current month)" << endl;
    cout << "========================================" << endl;

    return 0;
}

void virtualViaPointer(const Employee* const baseClassPtr, int currentMonth) {
    double baseEarnings = baseClassPtr->earnings();
    double totalEarnings = baseEarnings;

    baseClassPtr->print();
    cout << "\nearned $" << baseEarnings;

    if (baseClassPtr->isBirthdayMonth(currentMonth)) {
        totalEarnings += 100.00;
        cout << " + $100.00 BIRTHDAY BONUS!";
    }

    cout << "\ntotal earned: $" << totalEarnings << "\n\n";
}
