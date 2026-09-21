#include <bits/stdc++.h>
#include "RationalNumber.h"
using namespace std;

//gcd Funtion
int Rational::gcd(int a, int b) const {
	if (b == 0) return a;
	return gcd(b, a % b);
}

//reduce Function
void Rational::reduce() {
    if (denominator == 0) {
        denominator = 1;
    }

    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }

    int divisor = gcd(numerator, denominator);
    numerator /= divisor;
    denominator /= divisor;
}

//Constructor Function
Rational::Rational(int num, int den) {
	numerator = num;
	denominator = (den == 0) ? 1 : den;
    reduce();
}

//Operator Overloading
Rational Rational::operator+(const Rational& r) const {
    int num = numerator * r.denominator + r.numerator * denominator;
    int den = denominator * r.denominator;
    return Rational(num, den);
}
Rational Rational::operator-(const Rational& r) const {
    int num = numerator * r.denominator - r.numerator * denominator;
    int den = denominator * r.denominator;
    return Rational(num, den);
}
Rational Rational::operator*(const Rational& r) const {
    int num = numerator * r.numerator;
    int den = denominator * r.denominator;
    return Rational(num, den);
}
Rational Rational::operator/(const Rational& r) const {
    int num = numerator * r.denominator;
    int den = denominator * r.numerator;
    return Rational(num, den);
}

//Comparison Operators Overloading
bool Rational::operator==(const Rational& r) const {
    return numerator * r.denominator == r.numerator * denominator;
}
bool Rational::operator!=(const Rational& r) const {
    return !(*this == r);
}
bool Rational::operator<(const Rational& r) const {
    return numerator * r.denominator < r.numerator * denominator;
}
bool Rational::operator<=(const Rational& r) const {
    return *this < r || *this == r;
}
bool Rational::operator>(const Rational& r) const {
    return !(*this <= r);
}
bool Rational::operator>=(const Rational& r) const {
    return !(*this < r);
}

//Stream Operators Overloading
ostream& operator<<(ostream& os, const Rational& r) {
    if (r.denominator == 1) {
        os << r.numerator;
    }
    else {
        os << r.numerator << "/" << r.denominator;
    }
    return os;
}

istream& operator>>(istream& is, Rational& r) {
    int num, den;
    char slash;
    is >> num >> slash >> den;
    r = Rational(num, den);
    return is;
}
