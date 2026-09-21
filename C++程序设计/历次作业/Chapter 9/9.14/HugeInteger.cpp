#include "HugeInteger.h"
#include <bits/stdc++.h>
using namespace std;

HugeInteger::HugeInteger(const string& num) {
	fromString(num);
}

void HugeInteger::fromString(const string& s) {
	// initialize
	for (int i = 0; i < MAX_DIGITS; ++i) digits[i] = 0;
	negative = false;

	// parse string: allow optional +/-, ignore non-digits
	string t;
	for (char c : s) {
		if (c == '+' || c == ' ') continue;
		if (c == '-') { negative = true; continue; }
		if (isdigit(static_cast<unsigned char>(c))) t.push_back(c);
	}
	if (t.empty()) t = "0";

	// keep at most MAX_DIGITS least-significant digits
	if ((int)t.size() > MAX_DIGITS) {
		t = t.substr(t.size() - MAX_DIGITS);
	}

	// fill digits (most-significant at index 0)
	int offset = MAX_DIGITS - (int)t.size();
	for (int i = 0; i < (int)t.size(); ++i) {
		digits[offset + i] = t[i] - '0';
	}

	normalize();
}

string HugeInteger::toString() const {
	if (isZero()) return "0";
	string s;
	if (negative) s.push_back('-');
	int i = 0;
	while (i < MAX_DIGITS && digits[i] == 0) ++i;
	for (; i < MAX_DIGITS; ++i) s.push_back(char('0' + digits[i]));
	return s;
}

void HugeInteger::input() {
	string s;
	if (cin >> s) fromString(s);
}

void HugeInteger::output() const {
	cout << toString();
}

void HugeInteger::normalize() {
	// if all digits zero, ensure negative == false
	bool allZero = true;
	for (int i = 0; i < MAX_DIGITS; ++i) if (digits[i] != 0) { allZero = false; break; }
	if (allZero) negative = false;
}

// absolute value copy
HugeInteger HugeInteger::abs() const {
	HugeInteger r = *this;
	r.negative = false;
	return r;
}

// compare magnitudes (absolute values): return -1,0,1
int HugeInteger::compareMagnitude(const HugeInteger& other) const {
	for (int i = 0; i < MAX_DIGITS; ++i) {
		if (digits[i] < other.digits[i]) return -1;
		if (digits[i] > other.digits[i]) return 1;
	}
	return 0;
}

void HugeInteger::addMagnitude(const HugeInteger& a, const HugeInteger& b) {
	int carry = 0;
	for (int i = MAX_DIGITS - 1; i >= 0; --i) {
		int sum = a.digits[i] + b.digits[i] + carry;
		digits[i] = sum % 10;
		carry = sum / 10;
	}
	normalize();
}

void HugeInteger::subtractMagnitude(const HugeInteger& a, const HugeInteger& b) {
	// assumes |a| >= |b|
	int borrow = 0;
	for (int i = MAX_DIGITS - 1; i >= 0; --i) {
		int diff = a.digits[i] - b.digits[i] - borrow;
		if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
		digits[i] = diff;
	}
	normalize();
}

bool HugeInteger::isZero() const {
	for (int i = 0; i < MAX_DIGITS; ++i) if (digits[i] != 0) return false;
	return true;
}

// Comparison predicates
bool HugeInteger::isEqualTo(const HugeInteger& other) const {
	if (negative != other.negative) return false;
	return compareMagnitude(other) == 0;
}

bool HugeInteger::isNotEqualTo(const HugeInteger& other) const {
	return !isEqualTo(other);
}

bool HugeInteger::isGreaterThan(const HugeInteger& other) const {
	if (negative != other.negative) return negative ? false : true;
	// same sign
	int cmp = compareMagnitude(other);
	if (!negative) return cmp > 0;
	else return cmp < 0; // both negative
}

bool HugeInteger::isLessThan(const HugeInteger& other) const {
	return !isGreaterThanOrEqualTo(other);
}

bool HugeInteger::isGreaterThanOrEqualTo(const HugeInteger& other) const {
	if (negative != other.negative) return negative ? false : true;
	int cmp = compareMagnitude(other);
	if (!negative) return cmp >= 0;
	else return cmp <= 0;
}

bool HugeInteger::isLessThanOrEqualTo(const HugeInteger& other) const {
	return !isGreaterThan(other);
}

// Arithmetic: add/sub modify *this
void HugeInteger::add(const HugeInteger& other) {
	HugeInteger a = *this;
	HugeInteger b = other;

	if (a.negative == b.negative) {
		addMagnitude(a, b);
		negative = a.negative;
	} else {
		int cmp = a.compareMagnitude(b);
		if (cmp >= 0) {
			subtractMagnitude(a, b);
			negative = a.negative;
		} else {
			subtractMagnitude(b, a);
			negative = b.negative;
		}
	}
	normalize();
}

void HugeInteger::sub(const HugeInteger& other) {
	HugeInteger a = *this;
	HugeInteger b = other;

	if (a.negative != b.negative) {
		// a - (-b) == a + b
		addMagnitude(a, b);
		negative = a.negative;
	} else {
		int cmp = a.compareMagnitude(b);
		if (cmp >= 0) {
			subtractMagnitude(a, b);
			negative = a.negative;
		} else {
			subtractMagnitude(b, a);
			negative = !a.negative;
		}
	}
	normalize();
}

HugeInteger HugeInteger::multiplyByDigit(int digit) const {
	HugeInteger res("0");
	if (digit == 0) return res;
	if (digit < 0 || digit > 9) digit = max(0, min(9, digit));
	// work with least-significant-first array
	vector<int> a_rev(MAX_DIGITS);
	for (int i = 0; i < MAX_DIGITS; ++i) a_rev[i] = digits[MAX_DIGITS - 1 - i];

	vector<int> r(MAX_DIGITS + 1, 0);
	int carry = 0;
	for (int i = 0; i < MAX_DIGITS; ++i) {
		int prod = a_rev[i] * digit + carry;
		r[i] = prod % 10;
		carry = prod / 10;
	}
	if (carry) r[MAX_DIGITS] = carry;

	// assign back least-significant MAX_DIGITS digits (truncate higher)
	for (int i = 0; i < MAX_DIGITS; ++i) res.digits[MAX_DIGITS - 1 - i] = r[i];
	res.normalize();
	return res;
}

void HugeInteger::multiply(const HugeInteger& other) {
	// use convolution on absolute digits (least-significant-first)
	vector<int> a_rev(MAX_DIGITS), b_rev(MAX_DIGITS);
	for (int i = 0; i < MAX_DIGITS; ++i) {
		a_rev[i] = digits[MAX_DIGITS - 1 - i];
		b_rev[i] = other.digits[MAX_DIGITS - 1 - i];
	}
	vector<int> r(2 * MAX_DIGITS, 0);
	for (int i = 0; i < MAX_DIGITS; ++i) {
		for (int j = 0; j < MAX_DIGITS; ++j) {
			r[i + j] += a_rev[i] * b_rev[j];
		}
	}
	// carry propagate
	for (int k = 0; k < (int)r.size(); ++k) {
		if (r[k] >= 10) {
			int carry = r[k] / 10;
			r[k] %= 10;
			if (k + 1 < (int)r.size()) r[k + 1] += carry;
		}
	}
	// store least-significant MAX_DIGITS digits (truncate higher digits)
	for (int i = 0; i < MAX_DIGITS; ++i) digits[MAX_DIGITS - 1 - i] = r[i];
	negative = (negative != other.negative) && !isZero();
	normalize();
}

void HugeInteger::shiftLeftByOneDecimal() {
	// multiply by 10 in fixed-width representation (drop overflow)
	for (int i = 0; i < MAX_DIGITS - 1; ++i) digits[i] = digits[i + 1];
	digits[MAX_DIGITS - 1] = 0;
}

// divmod: divides *this (dividend) by divisor; returns quotient and remainder (both absolute handled)
// if divisor == 0, quotient = 0, remainder = *this (unchanged)
void HugeInteger::divmod(const HugeInteger& divisor, HugeInteger& quotient, HugeInteger& remainder) const {
	quotient = HugeInteger("0");
	remainder = HugeInteger("0");
	if (divisor.isZero()) {
		cout << "HugeInteger::divmod error: division by zero\n";
		return;
	}

	HugeInteger absDividend = this->abs();
	HugeInteger absDivisor = divisor.abs();

	// long division (digit by digit)
	int qdigits[MAX_DIGITS] = { 0 };

	for (int i = 0; i < MAX_DIGITS; ++i) {
		// remainder = remainder * 10 + next digit
		remainder.shiftLeftByOneDecimal();
		remainder.digits[MAX_DIGITS - 1] = absDividend.digits[i];

		// find the largest digit d in 0..9 such that absDivisor * d <= remainder
		int low = 0, high = 9, best = 0;
		while (low <= high) {
			int mid = (low + high) / 2;
			HugeInteger prod = absDivisor.multiplyByDigit(mid);
			// compare prod and remainder by magnitude (both non-negative)
			if (remainder.compareMagnitude(prod) >= 0) {
				best = mid;
				low = mid + 1;
			} else {
				high = mid - 1;
			}
		}
		if (best > 0) {
			HugeInteger prod = absDivisor.multiplyByDigit(best);
			remainder.sub(prod); // remainder >= prod ensures non-negative result
		}
		qdigits[i] = best;
	}

	// put qdigits into quotient
	for (int i = 0; i < MAX_DIGITS; ++i) quotient.digits[i] = qdigits[i];

	// set signs: quotient sign = dividend.sign XOR divisor.sign (unless quotient is zero)
	quotient.normalize();
	quotient.negative = (this->negative != divisor.negative) && !quotient.isZero();

	// remainder sign matches dividend's sign (C/C++ remainder convention)
	remainder.normalize();
	remainder.negative = this->negative && !remainder.isZero();
}

void HugeInteger::divide(const HugeInteger& other) {
	HugeInteger q, r;
	divmod(other, q, r);
	*this = q;
}

void HugeInteger::modulus(const HugeInteger& other) {
	HugeInteger q, r;
	divmod(other, q, r);
	*this = r;
}