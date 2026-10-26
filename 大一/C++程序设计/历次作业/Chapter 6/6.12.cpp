#include <bits/stdc++.h>
using namespace std;

double calculateCharges(double hours) {
	double basic_charge = (int(hours) < hours ? int(hours) + 1 : int(hours)) / 2;
	//这里是如果不满一个小时就按照一个小时计数
	return (basic_charge >= 11.5 ? 10.00 : (hours <= 3 ? 2.00 : basic_charge + 0.5));
	//这里有点怪，因为不想写太多行了所以套了两个三目运算符
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题特殊要求：使用calculateCharges函数来计算
	//这个题目没有说输入数据只要求打印表格，所以直接cout了
	cout << setw(10) << left << "Car" << setw(5) << right << "Hours" << setw(10) << "Charge" << endl;
	cout << setw(10) << left << "1" << setw(5) << right << "1.5" << setw(10) << fixed << setprecision(2) << calculateCharges(1.5) << endl;
	cout << setw(10) << left << "2" << setw(5) << right << "4.0" << setw(10) << fixed << setprecision(2) << calculateCharges(4.0) << endl;
	cout << setw(10) << left << "3" << setw(5) << right << "24.0" << setw(10) << fixed << setprecision(2) << calculateCharges(24.0) << endl;
	cout << setw(10) << left << "TOTAL" << setw(5) << right << "29.5" << setw(10) << fixed << setprecision(2) << calculateCharges(1.5) + calculateCharges(4.0) + calculateCharges(24.0) << endl;
	return 0;
}
int main() {
	return 0;
}
