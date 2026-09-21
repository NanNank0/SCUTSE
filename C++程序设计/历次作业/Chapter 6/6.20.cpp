#include <bits/stdc++.h>
using namespace std;

int multiple(int first, int second) {
	return (first % second == 0 ? 1 : 0);//三目运算符真的很好用
}
int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题没要求输入输出格式
	int a, b;
	cin >> a >> b;
	cout << boolalpha << bool(multiple(a, b));
	//这里脑子宕机了没想到怎么输出true和false所以才出此下策，当然可以用以下代码代替
	/*
	if(multiple(a,b)){
		cout << "True";
	}else{
		cout << "False";
	}
	*/
	return 0;
}