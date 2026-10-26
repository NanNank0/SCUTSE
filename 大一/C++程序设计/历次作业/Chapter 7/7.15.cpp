#include <bits/stdc++.h>
using namespace std;
int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要输出2*3数组的初始化顺序
	vector<vector<int>> tax(2,vector<int>(3));
	for (size_t row = 0; row < tax.size(); ++row) {
		for (size_t column = 0; column < tax[row].size(); ++column) {
			tax[row][column] = 1;
			cout << "tax[ " << row << " ][ " << column << " ], ";
		}
		cout << endl;
	}
	return 0;
}