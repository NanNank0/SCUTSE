/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <QtWidgets>
#include <set>
#include <tuple>
#include "graph.h"

// 先保留文件中的顶点和边，再分别构建矩阵和链表进行独立计算
struct GraphData
{
    QStringList names;
    std::vector<std::tuple<int, int, int>> edges;
    bool directed = true;

    template <class G> G build() const
    {
        G g(names.size());
        for (auto [u, v, w] : edges)
        {
            g.add(u, v, w);
            // 无向边需要补上反向边，自环只保存一次
            if (!directed && u != v)
            {
                g.add(v, u, w);
            }
        }
        return g;
    }
};

// 文本格式：n m directed，随后n个无空白顶点名，再逐行给出u v weight
// 解析前先检查顶点数、边数和有向标志，避免非法输入进入算法
inline GraphData parseGraph(const QString& text)
{
    QString copy = text;
    QTextStream in(&copy);
    int n = 0, m = 0, d = -1;
    in >> n >> m >> d;
    if (in.status() != QTextStream::Ok || n < 1 || n > 100 || m < 0 || m > 10000 ||
        (d != 0 && d != 1))
    {
        throw std::runtime_error("首行应为 n(1~100) m directed(0/1)");
    }
    GraphData out;
    out.directed = d;
    std::set<QString> names;
    for (int i = 0; i < n; ++i)
    {
        QString name;
        in >> name;
        if (name.isEmpty() || !names.insert(name).second)
        {
            throw std::runtime_error("顶点名称为空或重复");
        }
        out.names << name;
    }
    // 记录已经出现的端点组合，用于拒绝重复边
    std::set<std::pair<int, int>> used;
    for (int i = 0; i < m; ++i)
    {
        int u = -1, v = -1, w = -1;
        in >> u >> v >> w;
        if (in.status() != QTextStream::Ok || u < 0 || u >= n || v < 0 || v >= n || w < 0)
        {
            throw std::runtime_error("边端点越界或权值非自然数");
        }
        // 无向边将较小编号放在前面，使正反两个输入方向视为同一条边
        auto key =
            out.directed ? std::make_pair(u, v) : std::make_pair(std::min(u, v), std::max(u, v));
        if (!used.insert(key).second)
        {
            throw std::runtime_error("重复边");
        }
        out.edges.push_back({u, v, w});
    }
    // 读完声明的边数后检查是否还有额外内容，避免边数与文件不符
    QString extra;
    in >> extra;
    if (!extra.isEmpty())
    {
        throw std::runtime_error("边数与文件内容不一致");
    }
    return out;
}

// 用 UTF-8 读取图文件，支持中文物品名称和中文路径
inline GraphData loadGraph(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("无法读取图数据");
    }
    return parseGraph(QString::fromUtf8(f.readAll()));
}

// 把算法返回的顶点编号转换为便于阅读的名称序列
inline QString namesFor(const GraphData& d, const std::vector<int>& order)
{
    QStringList s;
    for (int u : order)
    {
        s << d.names[u];
    }
    return s.join(" → ");
}

// 邻接矩阵中的点号表示无边，数字零仍表示真实存在的零权边
inline QString matrixText(const GraphData& d)
{
    auto g = d.build<MatrixGraph>();
    QString out = "    ";
    for (int i = 0; i < g.size(); ++i)
    {
        out += QString::number(i).rightJustified(5);
    }
    out += '\n';
    for (int u = 0; u < g.size(); ++u)
    {
        out += QString::number(u).rightJustified(3) + " ";
        std::vector<QString> cells(g.size(), "·");
        for (auto e : g.neighbors(u))
        {
            cells[e.to] = QString::number(e.weight);
        }
        for (auto& x : cells)
        {
            out += x.rightJustified(5);
        }
        out += '\n';
    }
    return out;
}

// 逐行输出邻接链表，括号中显示当前边的权值
inline QString listText(const GraphData& d)
{
    auto g = d.build<ListGraph>();
    QString out;
    for (int u = 0; u < g.size(); ++u)
    {
        out += QString::number(u) + " " + d.names[u] + " : ";
        for (auto e : g.neighbors(u))
        {
            out += QString("%1(%2)  ").arg(d.names[e.to]).arg(e.weight);
        }
        out += '\n';
    }
    return out;
}
