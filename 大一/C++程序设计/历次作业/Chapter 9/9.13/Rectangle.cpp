#include "Rectangle.h"
using namespace std;

//Constructor Function
Rectangle::Rectangle(double len, double wid,double posX, double posY) {
	fillChar = '*';
	perimeterChar = '@';
	setLength(len);
	setWidth(wid);
	setPosition(posX, posY);
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
		width = 1.0;
		cout << "Invalid! Setting width to 1.0\n";
	}
}
void Rectangle::setPosition(double posX, double posY) {
	x = posX;
	y = posY;
}


//Get Function
double Rectangle::getLength() const {
	return length;
}
double Rectangle::getWidth() const {
	return width;
}
double Rectangle::getX() const {
	return x;
}
double Rectangle::getY() const {
	return y;
}

//Caculation Function
double Rectangle::perimeter()const {
	return 2 * (length + width);
}
double Rectangle::area() const {
	return length * width;
}

//Drawing Character settings Function
void Rectangle::setFillCharacter(char c) {
	fillChar = c;
}
void Rectangle::setPerimeterCharacter(char c) {
	perimeterChar = c;
}

//Drawing Function
void Rectangle::draw() const {
	const int size = 25;
	char background[size][size];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j<size; j++) {
			background[i][j] = ' ';
		}
	}
	//boundary setting
	int left = static_cast<int>(x);
	int right = static_cast<int>(x + length);
	int bottom = static_cast<int>(y);
	int top = static_cast<int>(y + width);
	
	//makesure boundary valid
	left = max(0, min(left, size - 1));
	right = max(0, min(right, size - 1));
	bottom = max(0, min(bottom, size - 1));
	top = max(0, min(top, size - 1));

	//draw it
	for (int i = bottom; i < top; i++) {
		for (int j = left; j < right; j++) {
			bool isBoundary = (i == bottom || i == top-1 || j == left || j == right-1);
			if (isBoundary) {
				background[i][j] = perimeterChar;
			}
			else {
				background[i][j] = fillChar;
			}
		}
	}
	cout << "\n    ";
	for (int i = 0; i < size; i++) {
		if (i % 5 == 0) {
			cout << (i) << " ";
		}
		else {
			cout << "  ";
		}
	}
	cout << " X\n";
	cout << "   +";
	for (int i = 0; i < size; i++) {
		cout << "--";
	}
	cout << "+\n";
	for (int i = size - 1; i >= 0; i--) {
		if (i % 5 == 0) {
			cout << setw(3) << i << "|";
		}
		else {
			cout << "   |";
		}

		for (int j = 0; j < size; j++) {
			cout << background[i][j] << " ";
		}
		cout << "|\n";
	}

	cout << "   +";
	for (int i = 0; i < size; i++) {
		cout << "--";
	}
	cout << "+\n";
	cout << "   Y";
	for (int i = 0; i < size; i++) {
		cout << (i % 5 == 0 ? " " + to_string(i) : "  ");
	}
	cout << " X\n";

	//Rectangle Info
	cout << "\nRectangle Info:\n";
	cout << "  Position: (" << x << ", " << y << ")\n";
	cout << "  Size: " << length << " x " << width << "\n";
	cout << "  Perimeter: " << perimeter() << "\n";
	cout << "  Area: " << area() << "\n";
	cout << "  Border char: '" << perimeterChar << "', Fill char: '" << fillChar << "'\n";
}

//yeah I fell ambitious
//scale size Function
void Rectangle::scale(double factor) {
	if (factor > 0) {
		double newLength = length * factor;
		double newWidth = width * factor;
		length = newLength;
		width = newWidth;
		cout << "Scaled by factor " << factor << endl;
	}
	else {
		cout << "Scale factor must be positive\n";
	}
}

//Rotate Functtion
void Rectangle::rotate() {
	double temp = length;
	length = width;
	width = temp;
	cout << "Rectangle rotated (swapped length and width)\n";
}

//move Function
void Rectangle::move(double deltaX, double deltaY) {
	double newX = x + deltaX;
	double newY = y + deltaY;
	if (newX >= 0 && newX + length <= 25 &&
		newY >= 0 && newY + width <= 25) {
		x = newX;
		y = newY;
		cout << "Moved by (" << deltaX << ", " << deltaY << ")\n";
	}
	else {
		cout << "Move cause rectangle to go out of bounds\n";
	}
}