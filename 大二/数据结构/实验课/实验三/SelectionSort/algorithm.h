/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <algorithm>
#include <array>

// 选择排序：每轮从未排序部分挑出最小元素
inline void SelectionSort(std::vector<unsigned>& a)
{
    // 在未排序后缀中找最小值，与前缀之后的第一个位置交换
    for (size_t i = 0; i < a.size(); ++i)
    {
        // best 保存本轮最小元素的下标，初始假定当前位置最小
        size_t best = i;
        for (size_t j = i + 1; j < a.size(); ++j)
        {
            if (a[j] < a[best])
            {
                best = j;
            }
        }
        // 只有找到更小的元素时才交换，减少不必要的写入
        if (best != i)
        {
            std::swap(a[i], a[best]);
        }
    }
}
