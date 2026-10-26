#include <bits/stdc++.h>
using namespace std;

void print(vector<int>, int, int);

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求使用递归来输出一个数组
	vector<int> array = { 1,2,3,4,5,6,7,8,9,10 };
	print(array, 0, 10);
	return 0;
}

void print(vector<int> array, int start, int end) {
	if (start == end) {
		return;
	}
	cout << array[start]; 
	print(array, start + 1, end);
}