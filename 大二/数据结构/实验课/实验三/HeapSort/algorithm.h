/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <algorithm>
#include <array>

// 向下筛选：让指定根结点重新满足最大堆性质
inline void siftDown(std::vector<unsigned>& a, size_t root, size_t n)
{
    // 最大堆向下筛选，每次让较大的孩子上移
    while (root < n / 2)
    {
        size_t child = root * 2 + 1;
        // 左右孩子都存在时，选择值更大的孩子进行比较
        if (child + 1 < n && a[child] < a[child + 1])
        {
            ++child;
        }
        // 根结点已经不小于孩子时，当前子树无需继续调整
        if (a[root] >= a[child])
        {
            break;
        }
        std::swap(a[root], a[child]);
        root = child;
    }
}

// 堆排序入口：先建立最大堆，再反复取出堆顶最大值
inline void HeapSort(std::vector<unsigned>& a)
{
    // 从最后一个非叶子结点开始向前建堆
    for (size_t i = a.size() / 2; i > 0; --i)
    {
        siftDown(a, i - 1, a.size());
    }
    for (size_t n = a.size(); n > 1; --n)
    {
        // 把最大值放到有序后缀，后续筛选不再包含该位置
        std::swap(a[0], a[n - 1]);
        siftDown(a, 0, n - 1);
    }
}
