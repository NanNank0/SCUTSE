#include <bits/stdc++.h>
#include "Time.h"
using namespace std;

int main() {
    //Ciallo～ (∠・ω< )⌒★
    Date d1(31, 1, 2026);//case a for day to month
    cout << "Starting date: ";
    d1.print();
    cout << "\n时が动き出した\n";
    for (int i = 0; i < 5; i++) {
        d1.nextDay();
        d1.print();
        cout << "\n";
    }
    cout << "\n";
    Date d2(31, 12, 2026);
    cout << "Starting date: ";
    d2.print();
    cout << "\n时が动き出した\n";
    for (int i = 0; i < 5; i++) {
        d2.nextDay();
        d2.print();
        cout << "\n";
    }
    return 0;
}