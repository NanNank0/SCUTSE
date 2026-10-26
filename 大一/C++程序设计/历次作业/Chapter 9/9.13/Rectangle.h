#ifndef RECTANGLE
#define RECTANGLE

#include <bits/stdc++.h>
using namespace std;
class Rectangle {
public:
	Rectangle(double len = 1.0, double wid = 1.0, double posX = 0, double posY = 0);
	void setLength(double len);
	void setWidth(double wid);
	void setPosition(double x, double y);
	void setFillCharacter(char c);
	void setPerimeterCharacter(char c);
	void draw() const;
	double getLength() const;
	double getWidth() const;
	double getX() const;
	double getY() const;
	double perimeter() const;
	double area() const;
	void scale(double factor);
	void rotate();
	void move(double deltaX, double deltaY);
private:
	double length;
	double width;
	char fillChar;
	char perimeterChar;
	double x;
	double y;

	bool isValid(double value) const;
};
#endif // !RECTANGLE
