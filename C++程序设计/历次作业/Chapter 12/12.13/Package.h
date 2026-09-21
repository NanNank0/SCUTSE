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

	string getSenderName() const { return senderName; }
	string getSenderAddress() const { return senderAddress; }
	string getSenderCity() const { return senderCity; }
	string getSenderState() const { return senderState; }
	string getSenderZip() const { return senderZip; }

	string getRecipientName() const { return recipientName; }
	string getRecipientAddress() const { return recipientAddress; }
	string getRecipientCity() const { return recipientCity; }
	string getRecipientState() const { return recipientState; }
	string getRecipientZip() const { return recipientZip; }

	virtual double calculateCost() const;
	virtual void printMailingLabel() const;
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
	void printMailingLabel() const override;
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
	void printMailingLabel() const override;
};

#endif // !PACKAGE_H
