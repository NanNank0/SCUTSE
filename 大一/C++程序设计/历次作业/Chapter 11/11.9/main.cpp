#include <bits/stdc++.h>
#include "Package.h"
using namespace std;
int main() {
	//Ciallo～ (∠・ω< )⌒★
	cout << fixed << setprecision(2);

	//Sender & Recipient Information
    string senderName = "楠楠子";
    string senderAddress = "666Road";
    string senderCity = "NNK";
    string senderState = "IL";
    string senderZip = "114514";

    string recipientName = "菜菜";
    string recipientAddress = "999Road";
    string recipientCity = "NNK";
    string recipientState = "IL";
    string recipientZip = "1919810";

    //Package Parameters
    double weight = 10;
    double costPerWeight = 0.5;

    //Test for Base Package
    cout << "Normal Package:\n";
    Package normal(senderName, senderAddress, senderCity, senderState, senderZip,
        recipientName, recipientAddress, recipientCity, recipientState, recipientZip,
        weight, costPerWeight);
    cout << "calculationCost is : " << normal.calculateCost() << endl;

    //Test for Two-Day Package
    double flatFee = 5;
    cout << "\nTwo-Day Package : \n";
    TwoDayPackage twoDay(senderName, senderAddress, senderCity, senderState, senderZip,
        recipientName, recipientAddress, recipientCity, recipientState, recipientZip,
        weight, costPerWeight, flatFee);
    cout << "calculationCost is : " << twoDay.calculateCost() << endl;

    //Test for Overnight Package
    double extraFeePerWeight = 0.3;
    OvernightPackage overnight(senderName, senderAddress, senderCity, senderState, senderZip,
        recipientName, recipientAddress, recipientCity, recipientState, recipientZip,
        weight, costPerWeight, extraFeePerWeight);
    cout << "\nOvernight Package : \n";
    cout << "calculationCost is : " << overnight.calculateCost() << endl;


	return 0;
}