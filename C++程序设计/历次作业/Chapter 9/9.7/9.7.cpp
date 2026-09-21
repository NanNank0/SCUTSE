#include <bits/stdc++.h>
#include "Time.h"
using namespace std;

int main() {
    //Ciallo～ (∠・ω< )⌒★
    Time t1(10, 59, 55);//test for second/minute(case a)
    for (int i = 0; i < 10; i++) {
        cout << "test for second to minute\n";
        cout << "Current Time: ";
        t1.printStandard();
        cout << "\nUniversal Time: ";
        t1.printUniversal();
        cout << "\n";
        t1.tick();
    }

    Time t2(11, 59, 55);//test for minute/hour(case b)
    for (int i = 0; i < 10; i++) {
        cout << "test for second to minute\n";
        cout << "Current Time: ";
        t2.printStandard();
        cout << "\nUniversal Time: ";
        t2.printUniversal();
        cout << "\n";
        t2.tick();
    }

    Time t3(23, 59, 55);//test for hour/day(case c)
    for (int i = 0; i < 10; i++) {
        cout << "test for second to minute\n";
        cout << "Current Time: ";
        t3.printStandard();
        cout << "\nUniversal Time: ";
        t3.printUniversal();
        cout << "\n";
        t3.tick();
    }
    return 0;
}