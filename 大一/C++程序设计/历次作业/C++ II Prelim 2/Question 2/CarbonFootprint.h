#ifndef CARBONFOOTPRINT_H
#define CARBONFOOTPRINT_H

#include <bits/stdc++.h>
using namespace std;

class CarbonFootprint {
public:
    virtual double getCarbonFootprint() const = 0;
    virtual ~CarbonFootprint() = default;
};

class Building : public CarbonFootprint {
private:
    string buildingName;    
    double area;            
    double electricityUsage;
    double gasUsage;        

    static constexpr double ELECTRICITY_FACTOR = 0.5;
    static constexpr double GAS_FACTOR = 5.3;        

public:
    //Constructor
    Building(const string& name, double area, double electricity, double gas);
    //Calculate carbon footprint
    double getCarbonFootprint() const override;
    //Display information
    void displayInfo() const;
};

class Car : public CarbonFootprint {
private:
    string model;         
    double annualMileage; 
    double fuelEfficiency;
    bool isElectric;      

    static constexpr double GASOLINE_FACTOR = 2.31; 
    static constexpr double ELECTRIC_FACTOR = 0.15; 

public:
    //Constructor
    Car(const string& model, double mileage, double efficiency, bool electric);
    //Calculate carbon footprint
    double getCarbonFootprint() const override;
    //Display information
    void displayInfo() const;
};

class Bicycle : public CarbonFootprint {
private:
    string type;               
    double manufacturingEmissions;
    int yearsOfUse;            
    double dailyCommuteKm;     
    int daysPerYear;           

public:
    // Constructor / 构造函数
    Bicycle(const string& type, double manuEmissions, int years, double commute, int days);

    // Calculate carbon footprint (annual average)
    // 计算碳足迹（年均值）
    double getCarbonFootprint() const override;

    // Display information / 显示信息
    void displayInfo() const;
};

#endif // CARBONFOOTPRINT_H