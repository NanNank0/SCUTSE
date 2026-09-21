#include <bits/stdc++.h>
#include "Package.h"
using namespace std;
int main() {
    //Ciallo～ (∠・ω< )⌒★
    cout << fixed << setprecision(2);

    //Sender & Recipient Information
    TwoDayPackage package1(
        "楠楠子1世", "666 Rd", "NNK", "IL", "114514",
        "菜菜1世", "999 Rd", "NNK", "IL", "1919810",
        10.0, 0.50, 5.00
    );

    TwoDayPackage package2(
        "楠楠子2世", "123Main St", "Beijing", "BJ", "100000",
        "菜菜2世", "456Oak Ave", "Shanghai", "SH", "200000",
        8.0, 0.45, 4.00
    );

    OvernightPackage package3(
        "楠楠子3世", "789Pine Rd", "Guangzhou", "GD", "510000",
        "菜菜3世", "321Maple Dr", "Shenzhen", "GD", "518000",
        5.0, 0.60, 0.25
    );

    OvernightPackage package4(
        "楠楠子4世", "111Elm Blvd", "Chengdu", "SC", "610000",
        "菜菜4世", "222Cedar Ln", "Chongqing", "CQ", "400000",
        12.0, 0.55, 0.20
    );

    TwoDayPackage package5(
        "楠楠子5世", "888Golden Gate", "San Francisco", "CA", "94101",
        "菜菜5世", "111Forest Ave", "Portland", "OR", "97201",
        6.0, 0.40, 3.00
    );

    OvernightPackage package6(
        "楠楠子6世", "555Ocean Rd", "Tokyo", "TY", "1000001",
        "菜菜6世", "777Mountain St", "Osaka", "OS", "540000",
        7.5, 0.65, 0.30
    );

    vector<Package*> packages;
    packages.push_back(&package1);
    packages.push_back(&package2);
    packages.push_back(&package3);
    packages.push_back(&package4);
    packages.push_back(&package5);
    packages.push_back(&package6);

    double totalShippingCost = 0.0;
    int packageCount = 0;
    cout << "PROCESSING " << packages.size() << " PACKAGES" << endl;

    for (Package* pkg : packages) {
        packageCount++;

        cout << "\nPACKAGE " << packageCount << endl;
        pkg->printMailingLabel();

        double cost = pkg->calculateCost();
        cout << "\nSHIPPING COST: $" << cost << endl;
        totalShippingCost += cost;
    }

    cout << "Total packages processed: " << packageCount << endl;
    cout << "TOTAL SHIPPING COST: $" << totalShippingCost << endl;

    cout << "BREAKDOWN BY SERVICE TYPE" << endl;

    double twoDayTotal = 0.0;
    double overnightTotal = 0.0;
    int twoDayCount = 0;
    int overnightCount = 0;

    for (Package* pkg : packages) {
        if (dynamic_cast<TwoDayPackage*>(pkg)) {
            twoDayTotal += pkg->calculateCost();
            twoDayCount++;
        }
        else if (dynamic_cast<OvernightPackage*>(pkg)) {
            overnightTotal += pkg->calculateCost();
            overnightCount++;
        }
    }
    cout << "\nTwo-Day Packages: " << twoDayCount << " packages" << endl;
    cout << "  Total: $" << twoDayTotal << endl;
    cout << "  Average: $" << (twoDayCount > 0 ? twoDayTotal / twoDayCount : 0) << endl;

    cout << "\nOvernight Packages: " << overnightCount << " packages" << endl;
    cout << "  Total: $" << overnightTotal << endl;
    cout << "  Average: $" << (overnightCount > 0 ? overnightTotal / overnightCount : 0) << endl;

    return 0;
}