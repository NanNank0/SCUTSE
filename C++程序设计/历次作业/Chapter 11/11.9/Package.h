#ifndef PACKAGE_H
#define PACKAGE_H

#include<bits/stdc++.h>
using namespace std;

class Package {
protected:

	//sender information
	string senderName;
	string senderAddress;
	string senderCity;
	string senderState;
	string senderZip;

	//recipient information
	string recipientName;
	string recipientAddress;
	string recipientCity;
	string recipientState;
	string recipientZip;

	double weight;
	double costPerWeight;

public:
	//Constructor
	Package(const string& sName, const string& sAddress,
		const string& sCity, const string& sState,
		const string& sZip, const string& rName,
		const string& rAddress, const string& rCity,
		const string& rState, const string& rZip,
		double weight, double costPerWeight);

	//Virtulal destructor
	virtual ~Package() = default;

	virtual double calculateCost() const;
	double getWeight() const;
	double getCostPer() const;
};

//Two-Day Package
class TwoDayPackage : public Package {
private:
	double flatFee;
public:
	TwoDayPackage(const string& sName, const string& sAddress,
		const string& sCity, const string& sState,
		const string& sZip, const string& rName,
		const string& rAddress, const string& rCity,
		const string& rState, const string& rZip,
		double weight, double costPerWeight, double fee);
	double calculateCost() const override;
};

class OvernightPackage : public Package {
private:
	double extraFee;
public:
	OvernightPackage(const string& sName, const string& sAddress,
		const string& sCity, const string& sState,
		const string& sZip, const string& rName,
		const string& rAddress, const string& rCity,
		const string& rState, const string& rZip,
		double weight, double costPerWeight, double fee);

	double calculateCost() const override;
};

#endif // !PACKAGE_H
