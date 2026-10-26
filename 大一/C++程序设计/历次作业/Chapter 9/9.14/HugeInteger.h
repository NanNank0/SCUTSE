#ifndef HUGEINTEGER
#define HUGEINTEGER
#include <bits/stdc++.h>
using namespace std;

class HugeInteger {
public:
	static const int MAX_DIGITS = 40;

	// Constructor
	HugeInteger(const string& num = "0");

	// I/O Functions
	void input();
	void output() const;

	// Linear calculation Functions (mutate *this)
	void add(const HugeInteger& other);
	void sub(const HugeInteger& other);
	void multiply(const HugeInteger& other);
	void divide(const HugeInteger& other);
	void modulus(const HugeInteger& other);

	// Predicate comparison functions
	bool isEqualTo(const HugeInteger& other) const;
	bool isNotEqualTo(const HugeInteger& other) const;
	bool isGreaterThan(const HugeInteger& other) const;
	bool isLessThan(const HugeInteger& other) const;
	bool isGreaterThanOrEqualTo(const HugeInteger& other) const;
	bool isLessThanOrEqualTo(const HugeInteger& other) const;
	bool isZero() const;

private:
	int digits[MAX_DIGITS]; // digits[0] = most significant, digits[MAX_DIGITS-1] = least significant
	bool negative;

	// internal helpers
	void fromString(const string& s);
	string toString() const;
	void normalize(); // clear leading zeros and fix sign for zero
	HugeInteger abs() const;

	// magnitude helpers (operate on absolute values)
	int compareMagnitude(const HugeInteger& other) const; // -1,0,1
	void addMagnitude(const HugeInteger& a, const HugeInteger& b); // this = |a| + |b|
	void subtractMagnitude(const HugeInteger& a, const HugeInteger& b); // this = |a| - |b|, assume |a| >= |b|

	// helpers for division/multiplication
	HugeInteger multiplyByDigit(int digit) const; // absolute multiply by 0..9
	void shiftLeftByOneDecimal(); // multiply *this by 10 (within fixed width)
	void divmod(const HugeInteger& divisor, HugeInteger& quotient, HugeInteger& remainder) const;
};

#endif // !HUGEINTEGER