#include <bits/stdc++.h>
using namespace std;

int quotient(int a, int b) {//照搬了6.24的代码
	return (a - (a % b)) / b;
}

void reverse(int a) {
	for (int i = 1; i <= a; i *= 10) {
		cout << quotient(a, i) - 10 * quotient(a, 10 * i);
	}
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//写一个函数将输入的数字倒序输出
	int n;
	cin >> n;
	reverse(n);
	return 0;
}