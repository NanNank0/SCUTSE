#include <bits/stdc++.h>
using namespace std;

int quotient(int a, int b) {//功能1
	return (a - (a % b)) / b;
}

int remainder(int a,int b) {//功能2
	return a % b;
}

void pieces(int a) {//功能3
	for (int i = 10000; i >= 1; i /= 10) {
		if (quotient(a, i) != 0) {
			for (int j = i; j >= 1; j /= 10) {
				cout << quotient(a, j) - 10 * quotient(a, j * 10) << " ";
			}//假设输入是abcd，第一次输出a，第二次quotient就要输出ab了，所以要减去上一次的10倍
			break;//这里我的思路是判断输入数字的最大位数（i）
		}
	}
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求完成三个功能：1.a除以b的整数部分。2.a除以b的余数部分。3.接受一个1~32767的整数并且分开输出
	//3的输入输出格式：4562 : 4 5 6 2
	int n;
	cin >> n;
	pieces(n);

	return 0;
}