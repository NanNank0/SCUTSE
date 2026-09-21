#include <bits/stdc++.h>
#include"Rectangle.h"
using namespace std;

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//test for basic draw
	Rectangle r1(5, 3, 2, 2);
	r1.draw();
	cout << "\n\n\n";
	//tert for characters
	Rectangle r2(8, 4, 1, 3);
	r2.setFillCharacter('O');
	r2.setPerimeterCharacter('X');
	r2.draw();
	cout << "\n\n\n";
	//test for scale
	Rectangle r3(4, 3, 8, 8);
	r3.draw();
	cout << "\n\n\n";
	r3.scale(1.5);
	r3.draw();
	cout << "\n\n\n";
	//test for rotate
	Rectangle r4(4, 3, 8, 8);
	r4.draw();
	cout << "\n\n\n";
	r4.rotate();
	r4.draw();
	cout << "\n\n\n";
	//test for move
	Rectangle r5(4, 3, 8, 8);
	r5.draw();
	cout << "\n\n\n";
	r5.move(2, 2);
	r5.draw();
	cout << "\n\n\n";
	return 0;
}