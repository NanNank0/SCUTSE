#include <bits/stdc++.h>
using namespace std;

bool prime(int a) {
	for (int i = 2; i < a; i++) {
		if (a % i == 0) {
			return false;//一旦出现被整除的情况就不是质数了
		}
	}
	return true;
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题有三个要求：1.输入数字判断是否质数。2.输出2到10000之间的质数。3.看不懂不用写
	for (int i = 2; i <= 10000; i++) {
		if (prime(i)) {
			cout << i << " ";
		}
	}
	return 0;
}