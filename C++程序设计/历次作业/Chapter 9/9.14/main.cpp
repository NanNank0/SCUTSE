#include <bits/stdc++.h>
#include "HugeInteger.h"
using namespace std;

int main() {
    //Ciallo～ (∠・ω< )⌒★
    // Test 1: Addition and Subtraction
    cout << "=== Test 1: Addition and Subtraction ===" << endl;

    HugeInteger num1("12345");
    HugeInteger num2("6789");

    cout << "num1 = "; num1.output(); cout << endl;
    cout << "num2 = "; num2.output(); cout << endl;

    // Addition
    HugeInteger addResult = num1;
    addResult.add(num2);
    cout << "\nAddition: ";
    num1.output();
    cout << " + ";
    num2.output();
    cout << " = ";
    addResult.output();
    cout << endl;

    // Subtraction
    HugeInteger subResult = num1;
    subResult.sub(num2);
    cout << "Subtraction: ";
    num1.output();
    cout << " - ";
    num2.output();
    cout << " = ";
    subResult.output();
    cout << endl;

    // Test 2: Comparison Functions
    cout << "\n=== Test 2: Comparison Functions ===" << endl;

    HugeInteger a("500");
    HugeInteger b("1000");
    HugeInteger c("500");
    HugeInteger d("-500");

    cout << "a = "; a.output(); cout << endl;
    cout << "b = "; b.output(); cout << endl;
    cout << "c = "; c.output(); cout << endl;
    cout << "d = "; d.output(); cout << endl;

    cout << "\nComparisons:" << endl;
    cout << "a isEqualTo c: " << (a.isEqualTo(c) ? "true" : "false") << endl;
    cout << "a isLessThan b: " << (a.isLessThan(b) ? "true" : "false") << endl;
    cout << "b isGreaterThan a: " << (b.isGreaterThan(a) ? "true" : "false") << endl;
    cout << "d isLessThan a: " << (d.isLessThan(a) ? "true" : "false") << endl;
    cout << "a isZero: " << (a.isZero() ? "true" : "false") << endl;

    HugeInteger zero("0");
    cout << "zero isZero: " << (zero.isZero() ? "true" : "false") << endl;

    return 0;
}