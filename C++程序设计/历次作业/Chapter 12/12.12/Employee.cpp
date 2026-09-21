#include "Employee.h"
using namespace std;

Date::Date(int y, int m, int d) {
    setDate(y, m, d);
}

bool Date::isLeapYear(int y) const {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int Date::getDaysInMonth(int m, int y) const {
    const int daysPerMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (m == 2 && isLeapYear(y)) {
        return 29;
    }
    return daysPerMonth[m - 1];
}

void Date::setDate(int y, int m, int d) {
    if (m < 1 || m > 12) {
        cout << "Month must be 1-12";
        exit(1);
    }
    if (d < 1 || d > getDaysInMonth(m, y)) {
        cout << "Invalid day for given month/year";
        exit(1);
    }
    year = y;
    month = m;
    day = d;
}

int Date::getYear() const { return year; }
int Date::getMonth() const { return month; }
int Date::getDay() const { return day; }

bool Date::isMonth(int m) const {
    return month == m;
}

void Date::print() const {
    cout << toString();
}

string Date::toString() const {
    return to_string(year) + "-" +
        (month < 10 ? "0" : "") + to_string(month) + "-" +
        (day < 10 ? "0" : "") + to_string(day);
}

ostream& operator<<(ostream& out, const Date& d) {
    out << d.toString();
    return out;
}

// ========== Employee Base Class Implementation ==========

Employee::Employee(const string& first, const string& last,
    const string& ssn, const Date& birthday)
    : firstName(first), lastName(last),
    socialSecurityNumber(ssn), birthDate(birthday) {
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

void Employee::setBirthDate(const Date& birthday) {
    birthDate = birthday;
}

Date Employee::getBirthDate() const {
    return birthDate;
}

bool Employee::isBirthdayMonth(int month) const {
    return birthDate.isMonth(month);
}

void Employee::print() const {
    cout << getFirstName() << ' ' << getLastName()
        << "\nsocial security number: " << getSocialSecurityNumber()
        << "\nbirthday: " << birthDate;
}

//Salaried Emplyeeo

SalariedEmployee::SalariedEmployee(const string& first, const string& last,
    const string& ssn, const Date& birthday,
    double salary)
    : Employee(first, last, ssn, birthday) {
    setWeeklySalary(salary);
}

void SalariedEmployee::setWeeklySalary(double salary) {
    if (salary >= 0.0) {
        weeklySalary = salary;
    }
    else {
        cout << "Weekly salary must be >= 0.0";
        exit(1);
    }
}

double SalariedEmployee::getWeeklySalary() const {
    return weeklySalary;
}

double SalariedEmployee::earnings() const {
    return getWeeklySalary();
}

void SalariedEmployee::print() const {
    cout << "salaried employee: ";
    Employee::print();
    cout << "\nweekly salary: " << getWeeklySalary();
}

//Commission Employee
CommissionEmployee::CommissionEmployee(const string& first, const string& last,
    const string& ssn, const Date& birthday,
    double sales, double rate)
    : Employee(first, last, ssn, birthday) {
    setGrossSales(sales);
    setCommissionRate(rate);
}

void CommissionEmployee::setGrossSales(double sales) {
    if (sales >= 0.0) {
        grossSales = sales;
    }
    else {
        cout << "Gross sales must be >= 0.0";
        exit(1);
    }
}

double CommissionEmployee::getGrossSales() const {
    return grossSales;
}

void CommissionEmployee::setCommissionRate(double rate) {
    if (rate > 0.0 && rate < 1.0) {
        commissionRate = rate;
    }
    else {
        cout << "Commission rate must be > 0.0 and < 1.0";
        exit(0);
    }
}

double CommissionEmployee::getCommissionRate() const {
    return commissionRate;
}

double CommissionEmployee::earnings() const {
    return getCommissionRate() * getGrossSales();
}

void CommissionEmployee::print() const {
    cout << "commission employee: ";
    Employee::print();
    cout << "\ngross sales: " << getGrossSales()
        << "; commission rate: " << getCommissionRate();
}

//Base + Commission
BasePlusCommissionEmployee::BasePlusCommissionEmployee(const string& first, const string& last,
    const string& ssn, const Date& birthday,
    double sales, double rate, double salary)
    : CommissionEmployee(first, last, ssn, birthday, sales, rate) {
    setBaseSalary(salary);
}

void BasePlusCommissionEmployee::setBaseSalary(double salary) {
    if (salary >= 0.0) {
        baseSalary = salary;
    }
    else {
        cout << "Salary must be >= 0.0";
        exit(1);
    }
}

double BasePlusCommissionEmployee::getBaseSalary() const {
    return baseSalary;
}

double BasePlusCommissionEmployee::earnings() const {
    return getBaseSalary() + CommissionEmployee::earnings();
}

void BasePlusCommissionEmployee::print() const {
    cout << "base-salaried ";
    CommissionEmployee::print();
    cout << "; base salary: " << getBaseSalary();
}