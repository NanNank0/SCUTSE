#include <bits/stdc++.h>
using namespace std;

int solution(vector<int>&);
int available(vector<int>, int);

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//八皇后问题：在8*8的国际象棋棋盘中摆放八个皇后使其不会相互攻击
	vector<int> queen;//这里使用一维数组存储皇后位置，因为必定是每一行有且仅有一个皇后
	//所以皇后在数组中的顺序就是行坐标，数据就是列坐标
	cout << solution(queen) << endl;
	return 0;
}

int solution(vector<int> &queen) {
	int solutions = 0;//多解计数器
	for (int i = 0; i < 8; i++) {
		if (available(queen, i)) {
			if (queen.size() != 7) {
				queen.push_back(i);//如果满足条件就把这个列坐标加入数组
				solutions += solution(queen);//然后重复判断下一行的哪一个位置可以放皇后
				queen.pop_back();//删除掉最新的皇后
			}
			else {
				queen.push_back(i);
				solutions++;
				queen.pop_back();
			}
		}
	}
	return solutions;
}

int available(vector<int> queen, int cPosition) {//检测这个位置能不能放皇后
	for (int i = 0; i < queen.size(); i++) {//判断有没有皇后在这一列或斜线（因为存储方式的原因不用判断行）
		if (cPosition == queen[i] || queen.size() - i == abs(cPosition - queen[i])) {
			return 0;
		}
	}
	return 1;
}