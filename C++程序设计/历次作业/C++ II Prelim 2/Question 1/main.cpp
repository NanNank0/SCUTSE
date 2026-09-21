#include <bits/stdc++.h>
#include "Employee.h"
using namespace std;

int main() {
    //Ciallo～ (∠・ω< )⌒★
    vector<Employee*> employees;
    Date currentDate;

    cout << "Type The Date: " << endl;
    int year, month, day;
    cout << "Year: ";
    cin >> year;
    cout << "Month: ";
    cin >> month;
    cout << "Day: ";
    cin >> day;
    currentDate.setDate(year, month, day);

    cout << "Employee Type: 1-Clerk, 2-Manager, 3-Sales Person, 4-Clearner, 0-End" << endl;

    int type;
    while (true) {
        cout << "\nType The type of Employee: ";
        cin >> type;

        if (type == 0) {
            break;
        }

        string name, empId;
        cout << "Name: ";
        cin >> name;
        cout << "ID: ";
        cin >> empId;

        cout << "Hire Year: ";
        cin >> year;
        cout << "Hire Month: ";
        cin >> month;
        cout << "Hire Day: ";
        cin >> day;
        Date hireDate(year, month, day);

        switch (type) {
        case 1: { //Clerk
            double salary;
            cout << "Salary: ";
            cin >> salary;
            employees.push_back(new Clerk(name, empId, hireDate, salary));
            break;
        }
        case 2: { //Manager
            double salary, sales;
            cout << "Salary: ";
            cin >> salary;
            cout << "Sales: ";
            cin >> sales;
            employees.push_back(new Manager(name, empId, hireDate, salary, sales));
            break;
        }
        case 3: { //Sales Person
            double salary, sales;
            cout << "Salary: ";
            cin >> salary;
            cout << "Sales: ";
            cin >> sales;
            employees.push_back(new SalesPerson(name, empId, hireDate, salary, sales));
            break;
        }
        case 4: { //Cleaner
            double rate;
            int hours;
            cout << "Salary: ";
            cin >> rate;
            cout << "Work Hours: ";
            cin >> hours;
            employees.push_back(new Cleaner(name, empId, hireDate, rate, hours));
            break;
        }
        default:
            cout << "Error Type!" << endl;
            continue;
        }
        cout << "Finish." << endl;
    }

    for (Employee* emp : employees) {
        emp->displayInfo(currentDate);
        cout << endl;
    }

    for (Employee* emp : employees) {
        delete emp;
    }

    return 0;
}