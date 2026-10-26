#include <bits/stdc++.h>
#include"Rectangle.h"
using namespace std;

int main() {
	//Ciallo～ (∠・ω< )⌒★
	double lenth, width;
	cin >> lenth >> width;
	Rectangle r1(lenth, width);
	cout << "Length: " << r1.getLength() << "\n"
		<< "Width: " << r1.getWidth() << "\n"
		<< "Perimeter: " << r1.perimeter() << "\n"
		<< "area: " << r1.area() << "\n";
	return 0;
}