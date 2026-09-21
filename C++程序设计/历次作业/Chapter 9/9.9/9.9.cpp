#include <bits/stdc++.h>
#include "Time.h"
using namespace std;

int main() {
    //Ciallo～ (∠・ω< )⌒★
    Date_Time dt(31, 12, 2026, 23, 59, 55);
    cout << "Starting Time: ";
    dt.printStandard();
    cout << "\n\n";
    for (int i = 0; i < 10; i++) {
        cout << "Universal time: ";
        dt.printUniversal();
        cout << "\nStandard time: ";
        dt.printStandard();
        cout << "\n\n";
        dt.tick();
    }
    return 0;
}