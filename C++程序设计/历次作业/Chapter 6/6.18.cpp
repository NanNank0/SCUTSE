#include <bits/stdc++.h>
using namespace std;

int integerPower(int base, int exponent) {
	int value = base;
	for (int i = 1; i < exponent; i++) {
		value *= base;
		//进行累乘，循环次数是exponent-1（因为value本身就有一次方）
	}
	return value;
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题特殊要求：使用integerPower函数，不允许使用math库里面自带的函数
	//这题没要求输出格式，输入格式也没有，所以随便写输入输出了
	int l1l1l1l, lIlIlIl;
	cin >> l1l1l1l >> lIlIlIl;//噩梦变量名嘻嘻
	cout << integerPower(l1l1l1l, lIlIlIl) << endl;
	return 0;
}