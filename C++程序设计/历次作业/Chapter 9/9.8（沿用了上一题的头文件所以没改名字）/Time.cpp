#include <bits/stdc++.h>
#include "Time.h"
using namespace std;

//Constructor Function
Date::Date(int d, int m, int y) {
	setDate(d, m, y);
}

void Date::setDate(int d, int m, int y) {
	setDay(d);
	setMonth(m);
	setYear(y);
}

//set Day,Month,Year with validation Function
void Date::setDay(int d) {
	int maxDay = daysInMonth(month, year);
	if (d >= 1 && d <= maxDay) {
		day = d;
	}
	else {
		day = 1;//default if invalid
		cout << "Invalid! Setting day to 1st\n";
	}
}
void Date::setMonth(int m) {
	if (m >= 1 && m <= 12) {
		month = m;
	}
	else {
		month = 1;//default if invalid
		cout << "Invalid! Setting month to 1st\n";
	}
}
void Date::setYear(int y) {
	year = y;
}

//Get Function
int Date::getDay() const {
	return day;
}
int Date::getMonth() const {
	return month;
}
int Date::getYear() const {
	return year;
}//我草了怎么还有闰年。。。

//Check leap year Function
bool Date::yearIsLeap(int y) const {
	return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

//get Number of Days in Month Funtcion
int Date::daysInMonth(int m, int y) const {
	switch (m) {
	case 1:case 3:case 5:case 7:case 8:case 10:case 12:
		return 31;
	case 4:case 6:case 9:case 11:
		return 30;
	case 2:
		return yearIsLeap(y) ? 29 : 28;
	default:
		//这里很神奇不加这个default在过年的时候就会days invalid
		//不知道为什么，明明在那里是没有调用nextDay的
		//但是大概是month变成13导致的...
		return 31;
	}
}

//Day increment Function
void Date::nextDay() {
	day++;
	if (day > daysInMonth(month, year)) {
		day = 1;
		month++;
		if (month > 12) {
			month = 1;
			year++;
		}
	}
}

//Print function
void Date::print() const {
	cout << year << "." << month << "." << day;
}