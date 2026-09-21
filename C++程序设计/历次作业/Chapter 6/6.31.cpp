#include <bits/stdc++.h>
using namespace std;

int lcm(int a, int b) {
	for (int i = (a >= b ? a : b); i <= a * b; i++) {//从a和b中最大的一个一直找到a*b
		if (not(i % a or i % b)) {
			return i;
		}
	}
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//求两个数字的最小公倍数
	int a, b;
	cin >> a >> b;
	cout << lcm(a, b);
	return 0;
}