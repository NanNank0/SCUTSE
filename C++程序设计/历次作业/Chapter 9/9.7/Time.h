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
