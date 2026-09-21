#ifndef VECTOR_H
#define VECTOR_H

#include <bits/stdc++.h>
using namespace std;

class Vector {
    friend ostream& operator<<(ostream&, const Vector&);
    friend istream& operator>>(istream&, Vector&);

public:
    //Constructor
    Vector(int size = 0);
    Vector(const Vector& other);
    ~Vector();

    //Assignment Operator
    Vector& operator=(const Vector& other);

    //Calculation Operator
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    double operator*(const Vector& other) const; 

    //get Size
    int getSize() const;
    void setSize(int newSize);

private:
    double* elements;  
    int size;          
};

#endif