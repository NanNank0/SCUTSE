#include <bits/stdc++.h>
using namespace std;

bool flip() {
	return rand() % 2;
}
int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求使用自定义的flip函数来判断正反两面
	srand(time(0));//时间作为随机数种子防止重复
	int count = 0;
	for (int i = 1; i <= 100; i++) {
		if (flip()) {
			count += 1;
		}
	}
	cout << "tails : " << 100 - count << endl << "heads : " << count;
	return 0;
}