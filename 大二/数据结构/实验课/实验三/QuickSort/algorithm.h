/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <algorithm>
#include <array>

// 三路快速排序：处理左闭右开区间 [l, r)
inline void quickRange(std::vector<unsigned>& a, size_t l, size_t r)
{
    while (r - l > 1)
    {
        // 三数取中选择轴值；三路划分避免大量相同值反复递归
        unsigned x = a[l], y = a[l + (r - l) / 2], z = a[r - 1];
        unsigned pivot = std::max(std::min(x, y), std::min(std::max(x, y), z));
        // lt 左侧小于轴值，lt 到 i 之间等于轴值，gt 右侧大于轴值
        size_t lt = l, i = l, gt = r;
        while (i < gt)
        {
            // 小于轴值时放到左区间，同时推进左边界和扫描位置
            if (a[i] < pivot)
            {
                std::swap(a[lt++], a[i++]);
            }
            // 大于轴值时放到右区间，换回的元素尚未检查，i 暂不前进
            else if (a[i] > pivot)
            {
                std::swap(a[i], a[--gt]);
            }
            // 等于轴值时只推进扫描位置，最终相等区间不必继续递归
            else
            {
                ++i;
            }
        }
        // 仅递归较短区间，较长区间用循环，控制最坏辅助栈深
        if (lt - l < r - gt)
        {
            quickRange(a, l, lt);
            l = gt;
        }
        else
        {
            quickRange(a, gt, r);
            r = lt;
        }
    }
}

// 排序入口把整个数组交给区间排序函数
inline void QuickSort(std::vector<unsigned>& a)
{
    quickRange(a, 0, a.size());
}
