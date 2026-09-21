#include "CarbonFootprint.h"

//Building
Building::Building(const string& name, double area, double electricity, double gas)
    : buildingName(name), area(area), electricityUsage(electricity), gasUsage(gas) {
}

double Building::getCarbonFootprint() const {
    // Annual carbon footprint from electricity and natural gas
    double electricityEmissions = electricityUsage * ELECTRICITY_FACTOR;
    double gasEmissions = gasUsage * GAS_FACTOR;

    return electricityEmissions + gasEmissions;
}

void Building::displayInfo() const {
    cout << "Building Information：" << endl;
    cout << "  Name: " << buildingName << endl;
    cout << "  Area: " << area << " m2" << endl;
    cout << "  Electricity: " << electricityUsage << " kWh/year" << endl;
    cout << "  Natural Gas: " << gasUsage << " therms/year" << endl;
    cout << "  Carbon Footprint: " << getCarbonFootprint() << " kg CO2/year" << endl;
}

//Car
Car::Car(const string& model, double mileage, double efficiency, bool electric)
    : model(model), annualMileage(mileage), fuelEfficiency(efficiency), isElectric(electric) {
}

double Car::getCarbonFootprint() const {
    if (isElectric) {
        // Electric vehicle
        return annualMileage * ELECTRIC_FACTOR;
    }
    else {
        // Gasoline vehicle
        // Convert L/100km to total liters: (mileage * fuelEfficiency / 100)
        double totalLiters = annualMileage * fuelEfficiency / 100.0;
        return totalLiters * GASOLINE_FACTOR;
    }
}

void Car::displayInfo() const {
    cout << "Car Information：" << endl;
    cout << "  Model: " << model << endl;
    cout << "  Annual Mileage: " << annualMileage << " km" << endl;
    if (isElectric) {
        cout << "  Type: Electric Vehicle" << endl;
        cout << "  Efficiency: " << ELECTRIC_FACTOR << " kg CO2/km" << endl;
    }
    else {
        cout << "  Type: Gasoline Vehicle" << endl;
        cout << "  Fuel Efficiency: " << fuelEfficiency << " L/100km" << endl;
    }
    cout << "  Carbon Footprint: " << getCarbonFootprint() << " kg CO2/year" << endl;
}

//Bicycle
Bicycle::Bicycle(const string& type, double manuEmissions, int years, double commute, int days)
    : type(type), manufacturingEmissions(manuEmissions), yearsOfUse(years),
    dailyCommuteKm(commute), daysPerYear(days) {
}

double Bicycle::getCarbonFootprint() const {
    double annualManufacturing = manufacturingEmissions / yearsOfUse;
    return annualManufacturing;
}

void Bicycle::displayInfo() const {
    cout << "Bicycle Information：" << endl;
    cout << "  Type: " << type << endl;
    cout << "  Manufacturing Emissions: " << manufacturingEmissions << " kg CO2" << endl;
    cout << "  Expected Lifespan: " << yearsOfUse << " years" << endl;
    cout << "  Daily Commute: " << dailyCommuteKm << " km" << endl;
    cout << "  Days Used Per Year: " << daysPerYear << endl;
    cout << "  Carbon Footprint: " << getCarbonFootprint() << " kg CO2/year (amortized)" << endl;
}