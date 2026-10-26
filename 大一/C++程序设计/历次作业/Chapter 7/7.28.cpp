#include <bits/stdc++.h>
using namespace std;

bool palindromes(string&, int, int);

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求写一个递归函数来判断一个字符串是否为回文字符串
	string object;
	cin >> object;
	if (palindromes(object, 0,object.size()-1)) {
		cout << "This string is a Palindrome";
	}
	else {
		cout << "This string isn't a Palindrome";
	}
	return 0;
}

bool palindromes(string &object, int start, int end) {
	if (start >= end) {
		return true;
	}
	if (object[start] != object[end]) {
		return false;
	}
	return palindromes(object, start + 1, end - 1);
}