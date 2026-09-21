#include <bits/stdc++.h>
#include "Time.h"
using namespace std;

//Constructor Function
Date_Time::Date_Time(int d, int m, int y, int h, int min, int sec) {
	setDateTime(d, m, y, h, min, sec);
}

void Date_Time::setDateTime(int d, int m, int y, int h, int min, int sec) {
	setYear(y);
	setMonth(m);
	setDay(d);
	setHour(h);
	setMinute(min);
	setSecond(sec);
}

//set with validation Function
void Date_Time::setHour(int h) {
	hour = (h >= 0 && h < 24) ? h : 0;
}
void Date_Time::setMinute(int m) {
	minute = (m >= 0 && m < 60) ? m : 0;
}
void Date_Time::setSecond(int s) {
	second = (s >= 0 && s < 60) ? s : 0;
}
void Date_Time::setDay(int d) {
	int maxDay = daysInMonth(month, year);
	if (d >= 1 && d <= maxDay) {
		day = d;
	}
	else {
		day = 1;//default if invalid
		cout << "Invalid! Setting day to 1st\n";
	}
}
void Date_Time::setMonth(int m) {
	if (m >= 1 && m <= 12) {
		month = m;
	}
	else {
		month = 1;//default if invalid
		cout << "Invalid! Setting month to 1st\n";
	}
}
void Date_Time::setYear(int y) {
	year = y;
}

//Get Function
int Date_Time::getDay() const {
	return day;
}
int Date_Time::getMonth() const {
	return month;
}
int Date_Time::getYear() const {
	return year;
}//我草了怎么还有闰年。。。
int Date_Time::getHour() const {
	return hour;
}
int Date_Time::getMinute() const {
	return minute;
}
int Date_Time::getSecond() const {
	return second;
}

//Check leap year Function
bool Date_Time::yearIsLeap(int y) const {
	return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

//get Number of Days in Month Funtcion
int Date_Time::daysInMonth(int m, int y) const {
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
void Date_Time::nextDay() {
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

//Tick increment Function;
void Date_Time::tick() {
	second++;
	if (second >= 60) {
		second = 0;
		minute++;
		if (minute >= 60) {
			minute = 0;
			hour++;
			if (hour >= 24) {
				hour = 0;
				nextDay();
			}
		}
	}
}

//Print Universal Time Function
void Date_Time::printUniversal() const {
	cout << setfill('0')
		<< setw(4) << year << "-"
		<< setw(2) << month << "-"
		<< setw(2) << day << " "
		<< setw(2) << hour << ":"
		<< setw(2) << minute << ":"
		<< setw(2) << second;
}


//Print Standard Time Function
void Date_Time::printStandard() const {
	cout << setfill('0')
		<< setw(4) << year << "-"
		<< setw(2) << month << "-"
		<< setw(2) << day << " ";
	int temp = (hour == 0 || hour == 12) ? 12 : hour % 12;
	cout << setw(2) << temp << ":"
		<< setw(2) << minute << ":"
		<< setw(2) << second
		<< (hour < 12 ? " AM" : " PM");
}