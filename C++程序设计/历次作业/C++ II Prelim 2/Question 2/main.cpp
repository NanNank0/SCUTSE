#include <bits/stdc++.h>
#include "CarbonFootprint.h"
using namespace std;

int main() {
    //Ciallo～ (∠・ω< )⌒★
    cout << fixed << setprecision(2);
    
    // Create objects
    Building building("Office Building", 1000.0, 50000.0, 2000.0);
    Car car("Toyota Camry", 20000.0, 9.0, false);
    Bicycle bicycle("Mountain Bike", 200.0, 10, 5.0, 200);
    
    // Store pointers to these objects in vector of CarbonFootprint pointers
    vector<CarbonFootprint*> footprints;
    footprints.push_back(&building);
    footprints.push_back(&car);
    footprints.push_back(&bicycle);
    for (size_t i = 0; i < footprints.size(); ++i) {
        double footprint = footprints[i]->getCarbonFootprint();
        // Print identifying information and carbon footprint
        if (i == 0) {
            cout << "Building: ";
        } else if (i == 1) {
            cout << "Car: ";
        } else {
            cout << "Bicycle: ";
        }
        
        cout << footprint << " kg CO2/year" << endl;
    }
    
    return 0;
}