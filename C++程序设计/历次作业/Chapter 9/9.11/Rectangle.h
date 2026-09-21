#ifndef RECTANGLE
#define RECTANGLE

#include <bits/stdc++.h>
using namespace std;
class Rectangle {
public:
	Rectangle(double len = 1.0, double wid = 1.0);
	void setLength(double len);
	void setWidth(double wid);
	double getLength() const;
	double getWidth() const;
	double perimeter() const;
	double area() const;
private:
	double length;
	double width;

	bool isValid(double value) const;
};
#endif // !RECTANGLE
