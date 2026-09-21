#ifndef TIME_H
#define TIME_H

class Time {
public:
	Time(int hour = 0, int minute = 0, int second = 0);
	void setTime(int hour, int minute = 0, int second = 0);
	void setHour(int hour);
	void setMinute(int minute);
	void setSecond(int second);
	int getHour() const;
	int getMinute() const;
	int getSecond() const;
	void tick();
	void printUniversal() const;
	void printStandard() const;
private:
	int hour, minute, second;
};
#endif // !TIME_H

#ifndef DATE
#define DATE

#include<bits/stdc++.h>
using namespace std;
class Date {
public:
	Date(int day = 1, int month = 1, int year = 1);
	void setDate(int day, int month, int year);
	void setMonth(int month);
	void setDay(int day);
	void setYear(int year);
	int getMonth() const;
	int getDay() const;
	int getYear() const;
	void nextDay();
	void print() const;
private:
	int month;
	int day;
	int year;

	int daysInMonth(int month, int year) const;
	bool yearIsLeap(int year) const;
};

#endif // !DATE

#ifndef DATE_TIME
#define DATE_TIME
#include <bits/stdc++.h>
using namespace std;
class Date_Time {
public:
	Date_Time(int day = 1, int month = 1, int year = 1, 
		int hour = 0, int minute = 0, int second = 0);
	void setDateTime(int day, int month, int year,int hour, int minute, int second);
	void setMonth(int month);
	void setDay(int day);
	void setYear(int year);
	int getMonth() const;
	int getDay() const;
	int getYear() const;
	void nextDay();
	void print() const;
	void setHour(int hour);
	void setMinute(int minute);
	void setSecond(int second);
	int getHour() const;
	int getMinute() const;
	int getSecond() const;
	void tick();
	void printUniversal() const;
	void printStandard() const;
private:
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;

	int daysInMonth(int month, int year) const;
	bool yearIsLeap(int year) const;
};
#endif // !DATE_TIME
