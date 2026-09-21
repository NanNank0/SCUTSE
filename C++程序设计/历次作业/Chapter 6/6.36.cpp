#include <bits/stdc++.h>
using namespace std;

int exponentitation(int base, int exponent) {
	if (exponent) {//这里是一点小优化，当exponent不为0的时候都是true
		return base * exponentitation(base, exponent - 1);
	}
	else {
		return 1;
	}
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题特殊要求：使用递归计算base^exponent
	int base, exponent;
	cin >> base >> exponent;
	cout << exponentitation(base, exponent);
	return 0;
}