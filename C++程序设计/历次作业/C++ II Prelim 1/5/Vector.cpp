#include "Vector.h"
using namespace std;

//Constructor
Vector::Vector(int s) {
    if (s < 0) {
        size = 0;
    }
    else {
        size = s;
    }

    elements = new double[size];
    for (int i = 0; i < size; i++) {
        elements[i] = 0.0;
    }
}

//Copy Constructor
Vector::Vector(const Vector& other) {
    size = other.size;
    elements = new double[size];
    for (int i = 0; i < size; i++) {
        elements[i] = other.elements[i];
    }
}

//Destructor
Vector::~Vector() {
    delete[] elements;
}

//Assignment Operator
Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        if (size != other.size) {
            delete[] elements;
            size = other.size;
            elements = new double[size];
        }

        for (int i = 0; i < size; i++) {
            elements[i] = other.elements[i];
        }
    }
    return *this;
}

//Calculation Operator
Vector Vector::operator+(const Vector& other) const {
    if (size != other.size) {
        cout << "Error: The size of vector are not the same!";
        exit(1);
    }

    Vector result(size);
    for (int i = 0; i < size; i++) {
        result.elements[i] = elements[i] + other.elements[i];
    }
    return result;
}

Vector Vector::operator-(const Vector& other) const {
    if (size != other.size) {
        cout << "Error: The size of vector are not the same!";
        exit(1);
    }

    Vector result(size);
    for (int i = 0; i < size; i++) {
        result.elements[i] = elements[i] - other.elements[i];
    }
    return result;
}

double Vector::operator*(const Vector& other) const {
    if (size != other.size) {
        cout << "Error: The size of vector are not the same!";
        exit(1);
    }

    double result = 0.0;
    for (int i = 0; i < size; i++) {
        result += elements[i] * other.elements[i];
    }
    return result;
}

int Vector::getSize() const {
    return size;
}

void Vector::setSize(int newSize) {
    if (newSize != size) {
        delete[] elements;
        size = newSize;
        elements = new double[size];
        for (int i = 0; i < size; i++) {
            elements[i] = 0.0;
        }
    }
}

//Input operator >>
istream& operator>>(istream& in, Vector& v) {
    cout << "Please type the Dimention of vector: ";
    in >> v.size;

    delete[] v.elements;
    v.elements = new double[v.size];
    cout << "Please type " << v.size << " elements:\n";
    for (int i = 0; i < v.size; i++) {
        in >> v.elements[i];
    }

    return in;
}

//Output operator
ostream& operator<<(ostream& out, const Vector& v) {
    out << "(";
    for (int i = 0; i < v.size; i++) {
        out << v.elements[i];
        if (i < v.size - 1) {
            out << ", ";
        }
    }
    out << ")";
    return out;
}