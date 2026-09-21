#include "Hugeint.h"
using namespace std;

Hugeint::Hugeint(const string& number) {
	for (int i = 0; i < 30; i++) {
		digits[i] = 0;
	}
	int length = number.length();
	for (int i = 0; i < length && i < 30; i++) {
		digits[29 - i] = number[length - 1 - i] - '0';
	}
}

void Hugeint::normalize() {
	int shift = 0;
	while (shift < 30 && digits[shift] == 0) {
		shift++;
	}
	if (shift == 30) {
		for (int i = 0; i < 30; i++) {
			digits[i] = 0;
		}
		return;
	}
	if (shift > 0) {
		for (int i = shift; i < 30; i++) {
			digits[i - shift] = digits[i];
		}
		for (int i = 30 - shift; i < 30; i++) {
			digits[i] = 0;
		}
	}
}

Hugeint Hugeint::operator+(const Hugeint& other) const {
	Hugeint result;
	int carry = 0;
	for (int i = 29; i >= 0; i--) {
		int sum = digits[i] + other.digits[i] + carry;
		result.digits[i] = sum % 10;
		carry = sum / 10;
	}
	result.normalize();
	return result;
}

Hugeint Hugeint::operator-(const Hugeint& other) const {
	Hugeint result;
	int borrow = 0;
	for (int i = 29; i >= 0; i--) {
		int diff = digits[i] - other.digits[i] - borrow;
		if (diff < 0) {
			diff += 10;
			borrow = 1;
		}
		else {
			borrow = 0;
		}
		result.digits[i] = diff;
	}
	result.normalize();
	return result;
}

Hugeint Hugeint::operator*(const Hugeint& other) const {
	Hugeint result;
	int temp[60] = { 0 };

	for (int i = 29; i >= 0; i--) {
		for (int j = 29; j >= 0; j--) {
			int pos = (29 - i) + (29 - j);
			temp[pos] += digits[i] * other.digits[j];
		}
	}

	int carry = 0;
	for (int i = 0; i < 60; i++) {
		temp[i] += carry;
		carry = temp[i] / 10;
		temp[i] %= 10;
	}

	for (int i = 0; i < 30; i++) {
		result.digits[29 - i] = temp[i];
	}

	return result;
}

Hugeint Hugeint::operator/(const Hugeint& other) const {
	Hugeint result;
	Hugeint remainder;
	Hugeint ten("10");
	Hugeint zero("0");

	if (other == zero) {
		return zero;
	}

	if (*this < other) {
		return zero;
	}

	for (int i = 0; i < 30; i++) {
		remainder = remainder * ten;
		int digit = digits[i];
		Hugeint tempDigit(string(1, '0' + digit));
		remainder = remainder + tempDigit;

		int quotient_digit = 0;
		while (remainder >= other && quotient_digit <= 9) {
			remainder = remainder - other;
			quotient_digit++;
		}
		result.digits[i] = quotient_digit;
	}

	result.normalize();
	return result;
}

bool Hugeint::operator==(const Hugeint& other) const {
	for (int i = 0; i < 30; i++) {
		if (digits[i] != other.digits[i]) {
			return false;
		}
	}
	return true;
}

bool Hugeint::operator!=(const Hugeint& other) const {
	return !(*this == other);
}

bool Hugeint::operator<(const Hugeint& other) const {
	int pos1 = 0, pos2 = 0;
	while (pos1 < 30 && digits[pos1] == 0) pos1++;
	while (pos2 < 30 && other.digits[pos2] == 0) pos2++;

	int len1 = 30 - pos1;
	int len2 = 30 - pos2;

	if (len1 != len2) {
		return len1 < len2;
	}

	for (int i = 0; i < len1; i++) {
		if (digits[pos1 + i] != other.digits[pos2 + i]) {
			return digits[pos1 + i] < other.digits[pos2 + i];
		}
	}
	return false;
}

bool Hugeint::operator>(const Hugeint& other) const {
	return other < *this;
}

bool Hugeint::operator<=(const Hugeint& other) const {
	return !(*this > other);
}

bool Hugeint::operator>=(const Hugeint& other) const {
	return !(*this < other);
}

ostream& operator<<(ostream& os, const Hugeint& hugeint) {
	bool leading_zero = true;
	for (int i = 0; i < 30; i++) {
		if (hugeint.digits[i] != 0) {
			leading_zero = false;
		}
		if (!leading_zero) {
			os << hugeint.digits[i];
		}
	}
	if (leading_zero) {
		os << '0';
	}
	return os;
}