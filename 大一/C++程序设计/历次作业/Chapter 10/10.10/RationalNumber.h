#ifndef RATIONAL
#define	RATIONAL

#include <bits/stdc++.h>
using namespace std;

class Rational {
	friend ostream& operator<<(ostream& os, const Rational& r);
	friend istream& operator>>(istream& is, const Rational& r);
public:
	//Constructor Function
	Rational(int num = 0, int den = 1);

	//Operator Overloading
	Rational operator+(const Rational& r) const;
	Rational operator-(const Rational& r) const;
	Rational operator*(const Rational& r) const;
	Rational operator/(const Rational& r) const;

	//Comparison Operators Overloading
	bool operator==(const Rational& r) const;
	bool operator!=(const Rational& r) const;
	bool operator<(const Rational& r) const;
	bool operator<=(const Rational& r) const;
	bool operator>(const Rational& r) const;
	bool operator>=(const Rational& r) const;

private:
	int numerator;
	int denominator;
	void reduce();
	int gcd(int a, int b) const;
};
#endif // !RATIONAL
