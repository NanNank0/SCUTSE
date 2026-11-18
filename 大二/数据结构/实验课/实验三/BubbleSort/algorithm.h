/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <algorithm>
#include <array>

// 冒泡排序：通过相邻比较，把当前最大值逐步送到末尾
inline void BubbleSort(std::vector<unsigned>& a)
{
    // 每趟把最大值送到末端；一趟没有交换即可提前结束
    for (size_t end = a.size(); end > 1; --end)
    {
        // 记录本轮是否发生交换，用于判断是否可以提前结束
        bool changed = false;
        for (size_t j = 1; j < end; ++j)
        {
            if (a[j - 1] > a[j])
            {
                std::swap(a[j - 1], a[j]);
                changed = true;
            }
        }
        // 一整轮都没有交换，说明剩余部分已经有序
        if (!changed)
        {
            break;
        }
    }
}
