#include <bits/stdc++.h>
#include "Time.h"
using namespace std;

//Constructor Function
Time::Time(int hour, int minute, int second) {
	setTime(hour, minute, second);
}

//Set Time Function
void Time::setTime(int hour, int minute, int second) {
	setHour(hour);
	setMinute(minute);
	setSecond(second);
}

//Boundary Function
void Time::setHour(int h) {
	hour = (h >= 0 && h < 24) ? h : 0;
}
void Time::setMinute(int m) {
	minute = (m >= 0 && m < 60) ? m : 0;
}
void Time::setSecond(int s) {
	second = (s >= 0 && s < 60) ? s : 0;
}

//Get Function
int Time::getHour() const {
	return hour;
}
int Time::getMinute() const{
	return minute;
}
int Time::getSecond() const {
	return second;
}

//Tick increment Function;
void Time::tick() {
    second++;
    if (second >= 60) {
        second = 0;
        minute++;
        if (minute >= 60) {
            minute = 0;
            hour++;
            if (hour >= 24) {
                hour = 0;
            }
        }
    }
}

//Print Universal Time Function
void Time::printUniversal() const {
    cout << setfill('0') << setw(2) << hour << ":"
        << setw(2) << minute << ":"
        << setw(2) << second;
}

//Print Standard Time Function
void Time::printStandard() const {
    cout << ((hour == 0 || hour == 12) ? 12 : hour % 12)
        << ":" << setfill('0') << setw(2) << minute
        << ":" << setfill('0') << setw(2) << second
        << (hour < 12 ? "AM" : "PM");
}