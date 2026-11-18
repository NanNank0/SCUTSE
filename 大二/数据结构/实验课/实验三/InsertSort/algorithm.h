/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <algorithm>
#include <array>

// 直接插入排序：依次扩大已经排好序的前缀
inline void InsertSort(std::vector<unsigned>& a)
{
    // 将当前值插入已经有序的前缀，严格大于才右移，保留稳定性
    for (size_t i = 1; i < a.size(); ++i)
    {
        // 先保存待插入的值，避免右移元素时覆盖它
        unsigned value = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > value)
        {
            a[j] = a[j - 1];
            --j;
        }
        // 右移结束后，j 就是当前元素应放入的位置
        a[j] = value;
    }
}
