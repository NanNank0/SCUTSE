/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <QtWidgets>
#include <chrono>
#include <random>
#include <stdexcept>
#include "../InsertSort/algorithm.h"
#include "../BubbleSort/algorithm.h"
#include "../SelectionSort/algorithm.h"
#include "../MergeSort/algorithm.h"
#include "../QuickSort/algorithm.h"
#include "../HeapSort/algorithm.h"
#include "../RadixSort/algorithm.h"

// 统一保存英文名称、中文名称和排序函数指针，便于复用计时流程
struct SortAlgorithm
{
    const char* name;
    const char* chinese;
    void (*sort)(std::vector<unsigned>&);
};

inline const std::array<SortAlgorithm, 7> sortAlgorithms{
    {{"InsertSort", "插入排序", InsertSort},
     {"BubbleSort", "冒泡排序", BubbleSort},
     {"SelectionSort", "选择排序", SelectionSort},
     {"MergeSort", "归并排序", MergeSort},
     {"QuickSort", "快速排序", QuickSort},
     {"HeapSort", "堆排序", HeapSort},
     {"RadixSort", "基数排序", RadixSort}}};

// 一条记录对应某算法的一种输入规模，保留三次原始耗时
struct Timing
{
    int algorithm, n;
    std::array<qint64, 3> ns;

    double average() const
    {
        return (ns[0] + ns[1] + ns[2]) / 3.0;
    }
};

// 读取空白分隔的自然数，并检查是否超出 32 位无符号整数范围
inline std::vector<unsigned> readNumbers(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("无法读取数据文件");
    }
    auto tokens =
        QString::fromUtf8(f.readAll()).split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    std::vector<unsigned> a;
    a.reserve(tokens.size());
    for (const auto& t : tokens)
    {
        bool ok = false;
        auto x = t.toULongLong(&ok);
        if (!ok || x > UINT_MAX)
        {
            throw std::runtime_error("数据应为32位自然数");
        }
        a.push_back(unsigned(x));
    }
    return a;
}

// 固定随机种子后生成三组输入，便于其他人复现实验
inline void generateData(const QString& dir)
{
    QDir().mkpath(dir);
    std::mt19937 rng(20261118);
    // 直接对生成器输出取模，固定种子下跨标准库复现同一份数据
    for (int n : {100, 1000, 10000})
    {
        QFile f(dir + QString("/%1_data.txt").arg(n));
        if (!f.open(QIODevice::WriteOnly))
        {
            throw std::runtime_error("写数据失败");
        }
        QTextStream out(&f);
        for (int i = 0; i < n; ++i)
        {
            out << (rng() % 1000000U) << ((i + 1) % 10 == 0 ? '\n' : ' ');
        }
    }
}

// 对指定算法或全部算法执行相同的三组输入与三次正式计时
inline std::vector<Timing> benchmark(const QString& dataDir, int selected = -1)
{
    std::vector<Timing> result;
    for (int n : {100, 1000, 10000})
    {
        auto original = readNumbers(dataDir + QString("/%1_data.txt").arg(n));
        if (original.size() != size_t(n))
        {
            throw std::runtime_error("数据规模与文件名不符");
        }
        // 标准库排序只用于生成校验答案，不参与被测算法的实现
        auto expected = original;
        std::sort(expected.begin(), expected.end());
        for (int k = 0; k < 7; ++k)
        {
            if (selected < 0 || selected == k)
            {
                // 预热一次，正式记录不包含这次排序的时间
                auto warm = original;
                sortAlgorithms[k].sort(warm);
                Timing t{k, n, {}};
                for (int run = 0; run < 3; ++run)
                {
                    auto a = original;
                    // 输入复制在计时之外，各算法使用同一份原始序列
                    // 计时起点紧挨排序调用，避免把输入复制的开销算进去
                    auto start = std::chrono::steady_clock::now();
                    sortAlgorithms[k].sort(a);
                    auto finish = std::chrono::steady_clock::now();
                    t.ns[run] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start)
                                    .count();
                    // 完整比较结果序列，同时检查顺序和元素数量是否正确
                    if (a != expected)
                    {
                        throw std::runtime_error("排序结果校验失败");
                    }
                }
                result.push_back(t);
            }
        }
    }
    return result;
}

// 同时导出逐次耗时和平均耗时，报告中的数字可从原始记录复核
inline void saveTimings(const QString& dir, const std::vector<Timing>& data)
{
    QDir().mkpath(dir);
    QFile raw(dir + "/timings.csv"), avg(dir + "/averages.csv");
    if (!raw.open(QIODevice::WriteOnly) || !avg.open(QIODevice::WriteOnly))
    {
        throw std::runtime_error("写结果失败");
    }
    QTextStream r(&raw), a(&avg);
    r << "algorithm,n,run,time_ns,verified\n";
    a << "algorithm,n,average_ns\n";
    a.setRealNumberNotation(QTextStream::FixedNotation);
    a.setRealNumberPrecision(2);
    for (const auto& t : data)
    {
        for (int i = 0; i < 3; ++i)
        {
            r << sortAlgorithms[t.algorithm].name << ',' << t.n << ',' << i + 1 << ',' << t.ns[i]
              << ",true\n";
        }
        a << sortAlgorithms[t.algorithm].name << ',' << t.n << ',' << t.average() << '\n';
    }
}

// 从正式测量记录恢复数据，让截图与报告使用同一组结果
inline std::vector<Timing> loadTimings(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("无法读取计时结果");
    }
    QTextStream in(&f);
    in.readLine();
    std::vector<Timing> out;
    while (!in.atEnd())
    {
        auto x = in.readLine().split(',');
        if (x.size() != 5)
        {
            throw std::runtime_error("计时文件格式错误");
        }
        int k = -1;
        for (int i = 0; i < 7; ++i)
        {
            if (x[0] == sortAlgorithms[i].name)
            {
                k = i;
            }
        }
        bool nOK = false, rOK = false, tOK = false;
        int n = x[1].toInt(&nOK), r = x[2].toInt(&rOK);
        auto ns = x[3].toLongLong(&tOK);
        if (k < 0 || !nOK || !rOK || !tOK || r < 1 || r > 3 || ns < 0 || x[4] != "true")
        {
            throw std::runtime_error("非法计时行");
        }
        auto it = std::find_if(out.begin(), out.end(),
                               [&](const Timing& t)
                               {
                                   return t.algorithm == k && t.n == n;
                               });
        if (it == out.end())
        {
            out.push_back({k, n, {}});
            it = out.end() - 1;
        }
        // 文件中的测量轮次从 1 开始，数组下标从 0 开始
        it->ns[r - 1] = ns;
    }
    return out;
}

// 窗口只预览前 24 个元素，完整序列仍保留在数据文件中
inline QString preview(const std::vector<unsigned>& a)
{
    QStringList x;
    for (size_t i = 0; i < std::min(size_t(24), a.size()); ++i)
    {
        x << QString::number(a[i]);
    }
    return x.join(' ') + (a.size() > 24 ? " …" : "");
}
