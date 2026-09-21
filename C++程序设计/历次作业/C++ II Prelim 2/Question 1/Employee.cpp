#include "Employee.h"
using namespace std;

//Date
bool Date::isLeapYear(int y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::getDaysInMonth(int y, int m) const {
    static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (m == 2 && isLeapYear(y)) return 29;
    return days[m - 1];
}

Date::Date(int y, int m, int d) : year(y), month(m), day(d) {}

Date::Date(const Date& other) : year(other.year), month(other.month), day(other.day) {}

void Date::setDate(int y, int m, int d) {
    year = y;
    month = m;
    day = d;
}

void Date::display() const {
    cout << year << "Äê" << month << "ÔÂ" << day << "ÈÕ";
}

//operator
int Date::operator-(const Date& other) const {
    Date start = other;
    Date end = *this;
    int days = 0;

    while (start.year < end.year ||
        (start.year == end.year && start.month < end.month) ||
        (start.year == end.year && start.month == end.month && start.day < end.day)) {
        days++;
        start.day++;

        if (start.day > getDaysInMonth(start.year, start.month)) {
            start.day = 1;
            start.month++;
            if (start.month > 12) {
                start.month = 1;
                start.year++;
            }
        }
    }

    return days;
}

//Employee
Employee::Employee(const string& n, const string& id, const Date& hire)
    : name(n), empID(id), hireDate(hire) {
}

double Employee::calculateSeniorityBonus(const Date& currentDate) const {
    int daysWorked = currentDate - hireDate;
    return static_cast<double>(daysWorked);
}

void Employee::displayInfo(const Date& currentDate) const {
    cout << "TYPE£º" << getEmployeeType() << endl;
    cout << "NAME£º" << name << endl;
    cout << "ID£º" << empID << endl;
    cout << "HIREDATE£º";
    hireDate.display();
    cout << endl;
    cout << "EARN£º" << fixed << setprecision(2) << calculateIncome(currentDate) << "$" << endl;
}

//Clerk
Clerk::Clerk(const string& n, const string& id, const Date& hire, double salary)
    : Employee(n, id, hire), fixedSalary(salary) {
}

double Clerk::calculateIncome(const Date& currentDate) const {
    return fixedSalary + calculateSeniorityBonus(currentDate);
}

//Manager
Manager::Manager(const string& n, const string& id, const Date& hire,
    double salary, double sales)
    : Employee(n, id, hire), fixedSalary(salary), departmentSales(sales) {
}

double Manager::calculateIncome(const Date& currentDate) const {
    double bonus = departmentSales * 0.02;
    return fixedSalary + bonus + calculateSeniorityBonus(currentDate);
}

//SalesPearson
SalesPerson::SalesPerson(const string& n, const string& id, const Date& hire,
    double salary, double sales)
    : Employee(n, id, hire), baseSalary(salary), personalSales(sales) {
}

double SalesPerson::calculateIncome(const Date& currentDate) const {
    double commission = personalSales * 0.01;
    return baseSalary + commission + calculateSeniorityBonus(currentDate);
}

//Cleaner
Cleaner::Cleaner(const string& n, const string& id, const Date& hire,
    double rate, int hours)
    : Employee(n, id, hire), hourlyRate(rate), hoursWorked(hours) {
}

double Cleaner::calculateIncome(const Date& currentDate) const {
    return hourlyRate * hoursWorked + calculateSeniorityBonus(currentDate);
}