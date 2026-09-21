#ifndef HUGEINT_H
#define HUGEINT_H

#include <bits/stdc++.h>
using namespace std;

class Hugeint {
	friend ostream& operator<<(ostream&, const Hugeint&);
public:
	Hugeint() {
		for (int i = 0; i < 30; i++) {
			digits[i] = 0;
		}
	}
	Hugeint(const string&);
	Hugeint(long num);

	Hugeint operator+(const Hugeint&) const;
	Hugeint operator-(const Hugeint&) const;
	Hugeint operator*(const Hugeint&) const;
	Hugeint operator/(const Hugeint&) const;

	bool operator==(const Hugeint&) const;
	bool operator!=(const Hugeint&) const;
	bool operator<(const Hugeint&) const;
	bool operator>(const Hugeint&) const;
	bool operator<=(const Hugeint&) const;
	bool operator>=(const Hugeint&) const;

private:
	int digits[30];
	void normalize();
};

#endif