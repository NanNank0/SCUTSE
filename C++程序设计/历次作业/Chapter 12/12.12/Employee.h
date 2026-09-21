#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <bits/stdc++.h>
using namespace std;

class Date {
    friend ostream& operator<<(ostream&, const Date&);

private:
    int year;
    int month;
    int day;

    bool isLeapYear(int y) const;
    int getDaysInMonth(int m, int y) const;

public:
    Date(int y = 1900, int m = 1, int d = 1);
    void setDate(int y, int m, int d);
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    bool isMonth(int m) const;
    void print() const;
    string toString() const;
};

//Base Class Employee
class Employee {
private:
    string firstName;
    string lastName;
    string socialSecurityNumber;
    Date birthDate;

public:
    Employee(const string&, const string&, const string&, const Date&);
    virtual ~Employee() {}

    void setFirstName(const string&);
    string getFirstName() const;
    void setLastName(const string&);
    string getLastName() const;
    void setSocialSecurityNumber(const string&);
    string getSocialSecurityNumber() const;
    void setBirthDate(const Date&);
    Date getBirthDate() const;
    bool isBirthdayMonth(int month) const;

    virtual double earnings() const = 0;
    virtual void print() const;
};

//Salaried Employee
class SalariedEmployee : public Employee {
private:
    double weeklySalary;

public:
    SalariedEmployee(const string&, const string&, const string&,
        const Date&, double = 0.0);
    virtual ~SalariedEmployee() {}

    void setWeeklySalary(double);
    double getWeeklySalary() const;

    virtual double earnings() const override;
    virtual void print() const override;
};

//Comission Employee
class CommissionEmployee : public Employee {
private:
    double grossSales;
    double commissionRate;

public:
    CommissionEmployee(const string&, const string&, const string&,
        const Date&, double = 0.0, double = 0.0);
    virtual ~CommissionEmployee() {}

    void setCommissionRate(double);
    double getCommissionRate() const;

    void setGrossSales(double);
    double getGrossSales() const;

    virtual double earnings() const override;
    virtual void print() const override;
};

//Base + Commission
class BasePlusCommissionEmployee : public CommissionEmployee {
private:
    double baseSalary;

public:
    BasePlusCommissionEmployee(const string&, const string&, const string&,
        const Date&, double = 0.0, double = 0.0, double = 0.0);
    virtual ~BasePlusCommissionEmployee() {}

    void setBaseSalary(double);
    double getBaseSalary() const;

    virtual double earnings() const override;
    virtual void print() const override;
};

#endif // EMPLOYEE_H