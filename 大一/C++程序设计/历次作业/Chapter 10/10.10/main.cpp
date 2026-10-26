#include <bits/stdc++.h>
#include "RationalNumber.h"
using namespace std;

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//Test Rational Class
	Rational r1(3, 4);
	Rational r2(1, 2);
	Rational r3(6, 8);
	Rational r4(2, 3);

	cout << "R1: " << r1 << endl;
	cout << "R2: " << r2 << endl;
	cout << "R3: " << r3 << endl;
	cout << "R4: " << r4 << endl << endl;

	//Test Operators
	cout << "R1 + R2: " << r1 + r2 << endl;
	cout << "R1 - R2: " << r1 - r2 << endl;
	cout << "R1 * R2: " << r1 * r2 << endl;
	cout << "R1 / R2: " << r1 / r2 << endl << endl;

	//Test Comparison Operators
	cout << "R1 == R2: " << (r1 == r2) << endl;
	cout << "R1 != R2: " << (r1 != r2) << endl;
	cout << "R1 < R2: " << (r1 < r2) << endl;
	cout << "R1 <= R2: " << (r1 <= r2) << endl;
	cout << "R1 > R2: " << (r1 > r2) << endl;
	cout << "R1 >= R2: " << (r1 >= r2) << endl;
	return 0;
}