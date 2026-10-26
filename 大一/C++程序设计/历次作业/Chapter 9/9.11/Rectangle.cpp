#include "Rectangle.h"
using namespace std;

//Constructor Function
Rectangle::Rectangle(double len, double wid) {
	setLength(len);
	setWidth(wid);
}

//Validation checker Function
bool Rectangle::isValid(double value) const {
	return (value > 0 && value < 20);
}

//Set Function
void Rectangle::setLength(double len) {
	if (isValid(len)) {
		length = len;
	}
	else {
		length = 1.0;
		//default setting
		cout << "Invalid! Setting lenth to 1.0\n";
	}
}
void Rectangle::setWidth(double wid) {
	if (isValid(wid)) {
		width = wid;
	}
	else {
		length = 1.0;
		cout << "Invalid! Setting width to 1.0\n";
	}
}

//Get Function
double Rectangle::getLength() const {
	return length;
}
double Rectangle::getWidth() const {
	return width;
}

//Caculation Function
double Rectangle::perimeter()const {
	return 2 * (length + width);
}
double Rectangle::area() const {
	return length * width;
}