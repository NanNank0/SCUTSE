#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b) {//辗转相除求最大公因数
	return a % b ? gcd(b, a % b) : b;
}

int lcm(int a, int b) {//公式法求最小公倍数
	return a * b / gcd(a, b);
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求使用递归解决6.29最小公倍数的问题
	int a, b;
	cin >> a >> b;
	cout << lcm(a, b);
	return 0;
}