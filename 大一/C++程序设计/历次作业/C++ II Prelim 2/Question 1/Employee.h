#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <bits/stdc++.h>
using namespace std;

class Date {
private:
	int year;
	int month;
	int day;
	bool isLeapYear(int y) const;
	int getDaysInMonth(int y, int m) const;

public:
	//Constructor
	Date(int y = 2000, int m = 1, int d = 1);
	Date(const Date& other);
	//Functions
	int getYear() const { return year; }
	int getMonth() const { return month; }
	int getDay() const { return day; }

	void setDate(int y, int m, int d);
	void display() const;
	//Operator
	int operator-(const Date& other) const;
};

class Employee {
private:
	string name;
	string empID;
	Date hireDate;
public:
	//Constructor
	Employee(const string& n, const string& id, const Date& hire);
	virtual ~Employee() {}
	string getName() const { return name; }
	string getEmpID() const { return empID; }
	Date getHireDate() const { return hireDate; }
	double calculateSeniorityBonus(const Date& currentDate) const;
	virtual double calculateIncome(const Date& currentDate) const = 0;
	virtual string getEmployeeType() const = 0;
	virtual void displayInfo(const Date& currentDate) const;
};

//Clerk
class Clerk : public Employee {
private:
	double fixedSalary;

public:
	Clerk(const string& n, const string& id, const Date& hire, double salary);
	double calculateIncome(const Date& currentDate) const override;
	string getEmployeeType() const override { return "Clerk"; }
};

//Manager
class Manager : public Employee {
private:
	double fixedSalary;
	double departmentSales;

public:
	Manager(const string& n, const string& id, const Date& hire,
		double salary, double sales);
	double calculateIncome(const Date& currentDate) const override;
	string getEmployeeType() const override { return "Manager"; }
};

//Sales Person
class SalesPerson : public Employee {
private:
	double baseSalary;
	double personalSales;

public:
	SalesPerson(const string& n, const string& id, const Date& hire,
		double salary, double sales);
	double calculateIncome(const Date& currentDate) const override;
	string getEmployeeType() const override { return "Sales Person"; }
};

//Cleaner
class Cleaner : public Employee {
private:
	double hourlyRate;
	int hoursWorked;

public:
	Cleaner(const string& n, const string& id, const Date& hire,
		double rate, int hours);
	double calculateIncome(const Date& currentDate) const override;
	string getEmployeeType() const override { return "Cleaner"; }
};
#endif // !EMPLOYEE_H
