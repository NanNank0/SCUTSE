#include <bits/stdc++.h>
#include "Hugeint.h"
using namespace std;

int main() {
	//Ciallo～ (∠・ω< )⌒★
	string num1("123456789012345678901234567890");
	string num2("114514191981011451419198101145");
	Hugeint huge1(num1);
	Hugeint huge2(num2);
	cout << "Hugeint 1: " << huge1 << endl;
	cout << "Hugeint 2: " << huge2 << endl;
	//test addition
	Hugeint sum = huge1 + huge2;
	cout << "Sum: " << sum << endl;
	//test subtraction
	Hugeint diff = huge2 - huge1;
	cout << "Difference: " << diff << endl;
	//test multiplication
	string num3("114514");
	string num4("1919810");
	Hugeint huge3(num3);
	Hugeint huge4(num4);
	Hugeint product = huge3 * huge4;
	cout << "Hugeint 3: " << huge3 << endl;
	cout << "Hugeint 4: " << huge4 << endl;
	cout << "Product of int3 and int4: " << product << endl;
	//test division
	Hugeint quotient = huge2 / huge1;
	cout << "Quotient: " << quotient << endl;
	//test comparison
	cout << "huge1 == huge2: " << (huge1 == huge2) << endl;
	cout << "huge1 != huge2: " << (huge1 != huge2) << endl;
	cout << "huge1 < huge2: " << (huge1 < huge2) << endl;
	cout << "huge1 > huge2: " << (huge1 > huge2) << endl;
	cout << "huge1 <= huge2: " << (huge1 <= huge2) << endl;
	cout << "huge1 >= huge2: " << (huge1 >= huge2) << endl;
	return 0;
}