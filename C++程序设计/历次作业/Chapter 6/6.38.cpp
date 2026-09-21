#include <bits/stdc++.h>
using namespace std;

void Hanoi(int n,int start,int tool,int goal) {//这里不会用char类型所以没用
	if (n == 1) {
		cout << start << "→" << goal << endl;//如果只有一个就直接挪到目标杆
	}
	else {
		Hanoi(n - 1, start, goal, tool);//先将n-1个盘子移到辅助杆
		cout << start << "→" << goal << endl;//将第n个盘子移到目标杆
		Hanoi(n - 1, tool, start, goal);//将n-1个盘子移到目标杆
	}
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求使用递归解决汉诺塔问题
	int n;
	cin >> n;
	Hanoi(n, 1, 2, 3);
	return 0;
}