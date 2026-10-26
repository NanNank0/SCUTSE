#include "Utils.h"
#include <iostream>
#include <cmath>

// 用户输入与边界检查
int getIntInput(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < minVal || value > maxVal) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Enter a number between " << minVal << " and " << maxVal << ".\n";
        }
        else {
            return value;
        }
    }
}

// 曼哈顿距离
int manhattanDistance(int r1, int c1, int r2, int c2) {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}
