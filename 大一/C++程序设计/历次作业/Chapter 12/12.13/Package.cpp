#include "Package.h"
using namespace std;

//Base Class
//Constructor Function
Package::Package(const string& sName, const string& sAddress,
	const string& sCity, const string& sState,
	const string& sZip, const string& rName,
	const string& rAddress, const string& rCity,
	const string& rState, const string& rZip,
	double weight, double costPerWeight)
	: senderName(sName), senderAddress(sAddress),
	senderCity(sCity), senderState(sState), senderZip(sZip),
	recipientName(rName), recipientAddress(rAddress),
	recipientCity(rCity), recipientState(rState), recipientZip(rZip) {

	this->weight = weight;
	this->costPerWeight = costPerWeight;

}

double Package::calculateCost() const {
	return weight * costPerWeight;
}

double Package::getWeight() const {
	return weight;
}

double Package::getCostPer() const {
	return costPerWeight;
}

void Package::printMailingLabel() const {
	cout << "SHIPPING LABEL - Standard Package" << endl;
	cout << "FROM:" << endl;
	cout << "  " << senderName << endl;
	cout << "  " << senderAddress << endl;
	cout << "  " << senderCity << ", " << senderState << " " << senderZip << endl;
	cout << endl;
	cout << "TO:" << endl;
	cout << "  " << recipientName << endl;
	cout << "  " << recipientAddress << endl;
	cout << "  " << recipientCity << ", " << recipientState << " " << recipientZip << endl;
}
//Two-Day Package
//Constructor Function
TwoDayPackage::TwoDayPackage(const string& sName, const string& sAddress,
	const string& sCity, const string& sState,
	const string& sZip, const string& rName,
	const string& rAddress, const string& rCity,
	const string& rState, const string& rZip,
	double weight, double costPerWeight, double fee)
	: Package(sName, sAddress, sCity, sState, sZip,
		rName, rAddress, rCity, rState, rZip, weight, costPerWeight) {

	flatFee = fee;
}

double TwoDayPackage::calculateCost() const {
	return Package::calculateCost() + flatFee;
}

void TwoDayPackage::printMailingLabel() const {
	cout << "SHIPPING LABEL - Two-Day Delivery" << endl;
	cout << "FROM:" << endl;
	cout << "  " << senderName << endl;
	cout << "  " << senderAddress << endl;
	cout << "  " << senderCity << ", " << senderState << " " << senderZip << endl;
	cout << endl;
	cout << "TO:" << endl;
	cout << "  " << recipientName << endl;
	cout << "  " << recipientAddress << endl;
	cout << "  " << recipientCity << ", " << recipientState << " " << recipientZip << endl;
	cout << "SERVICE: Two-Day Delivery" << endl;
	cout << "FLAT FEE: $" << flatFee << endl;
}

//Overnight Package
//Constructor
OvernightPackage::OvernightPackage(const string& sName, const string& sAddr,
	const string& sCity, const string& sState,
	const string& sZip,
	const string& rName, const string& rAddr,
	const string& rCity, const string& rState,
	const string& rZip,
	double w, double costPer, double fee)
	: Package(sName, sAddr, sCity, sState, sZip,
		rName, rAddr, rCity, rState, rZip,
		w, costPer) {
	extraFee = fee;
}

double OvernightPackage::calculateCost() const {
	double totalCostPer = getCostPer() + extraFee;
	return getWeight() * totalCostPer;
}

void OvernightPackage::printMailingLabel() const {
	cout << "SHIPPING LABEL - Overnight Delivery" << endl;
	cout << "FROM:" << endl;
	cout << "  " << senderName << endl;
	cout << "  " << senderAddress << endl;
	cout << "  " << senderCity << ", " << senderState << " " << senderZip << endl;
	cout << endl;
	cout << "TO:" << endl;
	cout << "  " << recipientName << endl;
	cout << "  " << recipientAddress << endl;
	cout << "  " << recipientCity << ", " << recipientState << " " << recipientZip << endl;
	cout << "SERVICE: Overnight Delivery" << endl;
	cout << "EXTRA FEE PER OUNCE: $" << extraFee << endl;
	cout << "BASE COST PER OUNCE: $" << costPerWeight << endl;
}