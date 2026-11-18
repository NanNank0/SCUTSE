/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#include <iostream>
#include <random>
#include <limits>
#include "../InsertSort/algorithm.h"
#include "../BubbleSort/algorithm.h"
#include "../SelectionSort/algorithm.h"
#include "../MergeSort/algorithm.h"
#include "../QuickSort/algorithm.h"
#include "../HeapSort/algorithm.h"
#include "../RadixSort/algorithm.h"

// 用固定例子与随机输入对照独立结果，检查算法的正常情况和边界情况
int main()
{
    using Fn = void (*)(std::vector<unsigned>&);
    Fn functions[] = {InsertSort, BubbleSort, SelectionSort, MergeSort,
                      QuickSort,  HeapSort,   RadixSort};
    std::vector<std::vector<unsigned>> cases{
        {}, {0}, {2, 1}, {1, 1, 1}, {UINT_MAX, 0, 256, 255, 65536, UINT_MAX}};
    std::vector<unsigned> ascending(600);
    for (size_t i = 0; i < ascending.size(); ++i)
    {
        ascending[i] = unsigned(i);
    }
    cases.push_back(ascending);
    std::reverse(ascending.begin(), ascending.end());
    cases.push_back(ascending);
    std::mt19937 rng(20261118);
    for (int t = 0; t < 150; ++t)
    {
        std::vector<unsigned> a(rng() % 400);
        for (auto& x : a)
        {
            x = t % 2 ? rng() % 13 : rng();
        }
        cases.push_back(a);
    }
    for (auto fn : functions)
    {
        for (auto input : cases)
        {
            auto expected = input;
            std::sort(expected.begin(), expected.end());
            fn(input);
            if (input != expected)
            {
                std::cerr << "sort mismatch\n";
                return 1;
            }
        }
    }
    std::cout << "PASS: " << cases.size() * 7
              << " sorting cases (empty, duplicates, ordered, UINT_MAX, seeded random)\n";
}
