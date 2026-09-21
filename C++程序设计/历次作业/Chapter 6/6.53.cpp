#include <bits/stdc++.h>
using namespace std;

template <typename temp>

temp product(const temp a, const temp b) {
	return a * b;
}
int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求使用函数模板来计算两个数的积
	int a, b;
	long a2, b2;
	float a3, b3;
	cout << "int: ";
	cin >> a >> b;
	cout << product(a, b) << endl;
	cout << "long: ";
	cin >> a2 >> b2;
	cout << product(a2, b2) << endl;
	cout << "float: ";
	cin >> a3 >> b3;
	cout << product(a3, b3) << endl;
	return 0;
}