/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <algorithm>
#include <array>

// 低位优先基数排序：按无符号整数的字节逐轮排序
inline void RadixSort(std::vector<unsigned>& a)
{
    if (a.empty())
    {
        return;
    }
    static_assert(sizeof(unsigned) == 4, "Requires 32-bit unsigned");
    // 最大值决定需要处理多少个有效字节
    unsigned largest = *std::max_element(a.begin(), a.end());
    std::vector<unsigned> tmp(a.size());
    // LSD 基数排序：每轮处理一个字节，前缀计数后稳定分配
    for (unsigned shift = 0; shift < 32; shift += 8)
    {
        // 一个字节有 256 种取值，先统计每个桶应接收多少元素
        std::array<size_t, 256> counts{};
        for (auto x : a)
        {
            ++counts[(x >> shift) & 255U];
        }
        // 将桶内数量转换成各桶在临时数组中的起始下标
        size_t total = 0;
        for (auto& c : counts)
        {
            auto old = c;
            c = total;
            total += old;
        }
        // 按原顺序进入对应桶，保持当前这一轮分配的稳定性
        for (auto x : a)
        {
            tmp[counts[(x >> shift) & 255U]++] = x;
        }
        // 交换两个数组，让本轮输出成为下一轮的输入
        a.swap(tmp);
        // 防止右移 32 位；最高有效字节处理完后结束
        if (shift == 24 || (largest >> (shift + 8)) == 0)
        {
            break;
        }
    }
}
