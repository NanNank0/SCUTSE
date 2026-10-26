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
	
	this -> weight = weight;
	this -> costPerWeight = costPerWeight;
	
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

//Two-Day Package
//Constructor Function
TwoDayPackage::TwoDayPackage(const string& sName, const string& sAddress,
	const string& sCity, const string& sState,
	const string& sZip, const string& rName,
	const string& rAddress, const string& rCity,
	const string& rState, const string& rZip,
	double weight, double costPerWeight, double fee)
	: Package(sName, sAddress, sCity, sState, sZip,
		rName, rAddress, rCity, rState, rZip, weight, costPerWeight){

	flatFee = fee;
}

double TwoDayPackage::calculateCost() const {
	return Package::calculateCost() + flatFee;
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