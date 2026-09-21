#include <bits/stdc++.h>
using namespace std;

int second(int hours, int minutes, int seconds) {
	return (hours % 12) * 3600 + minutes * 60 + seconds;
}

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题无特殊要求
	int hours, minutes, seconds;
	cin >> hours >> minutes >> seconds;
	cout << second(hours, minutes, seconds);
	return 0;
}