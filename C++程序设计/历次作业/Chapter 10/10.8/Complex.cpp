#include<bits/stdc++.h>
#include "Complex.h"
using namespace std;

//Constructors Functions
Complex::Complex(double real, double imag) : real(real), imag(imag) {}//empty body

//Operators Overloading Functions
Complex Complex::operator+(const Complex& other) const {
	return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
	return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator*(const Complex& other) const {
	return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
}

//Comparison Operators Functions
bool Complex::operator==(const Complex& other) const {
	return real == other.real && imag == other.imag;
}
bool Complex::operator!=(const Complex& other) const {
	return !(*this == other);
}

//Stream Operators Functions
ostream& operator<<(ostream& out, const Complex& c) {
	out << c.real << " + " << c.imag << "i";
	return out;
}
