#ifndef DATE
#define DATE

#include <bits/stdc++.h>
using namespace std;

class Date {
    friend ostream& operator<<(ostream&, const Date&);
public:
    Date(int m = 1, int d = 1, int y = 1900);
    void setDate(int, int, int);
    int getMonth() const;
    int getDay() const;
    int getYear() const;
    bool isSameMonth(int month) const;
private:
    int month;
    int day;
    int year;
};

#endif

#ifndef EMPLOYEE
#define EMPLOYEE

using namespace std;

class Employee {
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

    virtual double earnings() const = 0;
    virtual void print() const;
    bool isBirthdayThisMonth(int currentMonth) const;

private:
    string firstName;
    string lastName;
    string socialSecurityNumber;
    Date birthDate; 
};
#endif // !EMPLOEE

// SalariedEmployee.h
#ifndef SALARIED
#define SALARIED

class SalariedEmployee : public Employee {
public:
    SalariedEmployee(const string&, const string&, const string&,
        const Date&, double = 0.0);
    virtual ~SalariedEmployee() {}

    void setWeeklySalary(double);
    double getWeeklySalary() const;

    virtual double earnings() const override;
    virtual void print() const override;

private:
    double weeklySalary;
};

#endif