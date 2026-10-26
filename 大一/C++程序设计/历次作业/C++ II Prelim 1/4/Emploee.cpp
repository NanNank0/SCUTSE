#include <bits/stdc++.h>
#include "Emploee.h"

using namespace std;

//Employee Functions
Employee::Employee(const string& first, const string& last,
    const string& ssn, const Date& birth)
    : firstName(first), lastName(last),
    socialSecurityNumber(ssn), birthDate(birth) {
}

void Employee::setFirstName(const string& first) {
    firstName = first;
}

string Employee::getFirstName() const {
    return firstName;
}

void Employee::setLastName(const string& last) {
    lastName = last;
}

string Employee::getLastName() const {
    return lastName;
}

void Employee::setSocialSecurityNumber(const string& ssn) {
    socialSecurityNumber = ssn;
}

string Employee::getSocialSecurityNumber() const {
    return socialSecurityNumber;
}

void Employee::setBirthDate(const Date& birth) {
    birthDate = birth;
}

Date Employee::getBirthDate() const {
    return birthDate;
}

bool Employee::isBirthdayThisMonth(int currentMonth) const {
    return birthDate.isSameMonth(currentMonth);
}

void Employee::print() const {
    cout << getFirstName() << ' ' << getLastName()
        << "\nSocial Security Number: " << getSocialSecurityNumber()
        << "\nBirthday: " << birthDate;
}

//Date Functions
Date::Date(int m, int d, int y) {
    setDate(m, d, y);
}

void Date::setDate(int m, int d, int y) {
    month = (m >= 1 && m <= 12) ? m : 1;
    day = (d >= 1 && d <= 31) ? d : 1;
    year = y;
}

int Date::getMonth() const { return month; }
int Date::getDay() const { return day; }
int Date::getYear() const { return year; }

bool Date::isSameMonth(int m) const {
    return month == m;
}

ostream& operator<<(ostream& os, const Date& d) {
    os << d.month << "/" << d.day << "/" << d.year;
    return os;
}

//Salaried Employee Functions
SalariedEmployee::SalariedEmployee(const string& first, const string& last,
    const string& ssn, const Date& birth, double salary)
    : Employee(first, last, ssn, birth) {
    setWeeklySalary(salary);
}

void SalariedEmployee::setWeeklySalary(double salary) {
    if (salary >= 0.0)
        weeklySalary = salary;
    else
        throw invalid_argument("Weekly Salary Must be >= 0.0");
}

double SalariedEmployee::getWeeklySalary() const {
    return weeklySalary;
}

double SalariedEmployee::earnings() const {
    return getWeeklySalary();
}

void SalariedEmployee::print() const {
    cout << "Emplyee: ";
    Employee::print();
    cout << "\nWeekly Salary: " << getWeeklySalary();
}