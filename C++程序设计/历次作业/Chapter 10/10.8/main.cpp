#include <bits/stdc++.h>
#include "Complex.h"
using namespace std;
int main() {
	//Ciallo～ (∠・ω< )⌒★

	Complex x, y(4.3, 8.2), z(3.3, 1.1);
	//test for Addition
	x = y + z;
	cout << "y + z = " << x << endl;
	//test for Subtraction
	x = y - z;
	cout << "y - z = " << x << endl;
	//test for Multiplication
	x = y * z;
	cout << "y * z = " << x << endl;
	//test for Comparison
	if (y == z) {
		cout << "y and z are equal." << endl;
	}
	else if(y != z) {
		cout << "y and z are not equal." << endl;
	}
	return 0;
}