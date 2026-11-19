/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <algorithm>
#include <array>

// 递归处理左闭右开区间 [l, r)，长度不足两个时无需排序
inline void mergeRange(std::vector<unsigned>& a, std::vector<unsigned>& tmp, size_t l, size_t r)
{
    if (r - l < 2)
    {
        return;
    }
    // 取中点后分别排序左右两半，再把两个有序区间合并
    const auto m = l + (r - l) / 2;
    mergeRange(a, tmp, l, m);
    mergeRange(a, tmp, m, r);
    size_t i = l, j = m, k = l;
    // 相等时优先取左半段，保证归并排序稳定
    while (i < m && j < r)
    {
        tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];
    }
    // 其中一半已经取完后，把另一半尚未处理的元素接到后面
    while (i < m)
    {
        tmp[k++] = a[i++];
    }
    while (j < r)
    {
        tmp[k++] = a[j++];
    }
    // 临时数组保存本轮归并结果，最后写回原数组对应区间
    std::copy(tmp.begin() + l, tmp.begin() + r, a.begin() + l);
}

// 临时数组只在入口申请一次，递归过程中反复复用
inline void MergeSort(std::vector<unsigned>& a)
{
    std::vector<unsigned> tmp(a.size());
    mergeRange(a, tmp, 0, a.size());
}
