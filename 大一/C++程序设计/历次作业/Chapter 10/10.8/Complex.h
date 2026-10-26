#ifndef COMPLEX
#define COMPLEX

#include <bits/stdc++.h>
using namespace std;

class Complex
{
	friend ostream& operator<<(ostream& out, const Complex& c);
public:
	explicit Complex(double real = 0.0, double imag = 0.0);//Constructor with default values

	//operators overloading
	Complex operator+(const Complex& c) const;
	Complex operator-(const Complex& c) const;
	Complex operator*(const Complex& c) const;

	//comparison operators
	bool operator==(const Complex& c) const;
	bool operator!=(const Complex& c) const;

private:
	double real;
	double imag;
};
#endif // !COMPLEX
