/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include <vector>
#include <list>
#include <optional>
#include <queue>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <functional>

// 一条邻接边保存终点编号和权值，边的起点由所在行或链表决定
struct Edge
{
    int to, weight;
};

// 邻接矩阵：optional 区分不存在的边与权重为0的边
class MatrixGraph
{
    std::vector<std::vector<std::optional<int>>> cells;

  public:
    explicit MatrixGraph(int n) : cells(n, std::vector<std::optional<int>>(n))
    {
    }

    int size() const
    {
        return int(cells.size());
    }

    void add(int u, int v, int w)
    {
        cells.at(u).at(v) = w;
    }

    // 逐列扫描矩阵中的一行，按编号收集实际存在的出边
    std::vector<Edge> neighbors(int u) const
    {
        std::vector<Edge> out;
        for (int v = 0; v < size(); ++v)
        {
            if (cells.at(u)[v])
            {
                out.push_back({v, *cells[u][v]});
            }
        }
        return out;
    }
};

// 邻接链表：每个顶点对应一条真实链表，按目标编号递增存储
class ListGraph
{
    std::vector<std::list<Edge>> lists;

  public:
    explicit ListGraph(int n) : lists(n)
    {
    }

    int size() const
    {
        return int(lists.size());
    }

    void add(int u, int v, int w)
    {
        // 在有序邻接链表中查找插入位置，保证两种表示的访问顺序一致
        auto& l = lists.at(u);
        auto it = l.begin();
        while (it != l.end() && it->to < v)
        {
            ++it;
        }
        if (it != l.end() && it->to == v)
        {
            it->weight = w;
        }
        else
        {
            l.insert(it, {v, w});
        }
    }

    // 返回链表的常量引用，遍历时不复制整条邻接链表
    const std::list<Edge>& neighbors(int u) const
    {
        return lists.at(u);
    }
};

// 外层循环保证不连通图的全部顶点也会被访问，先访问指定起点所在分量
template <class G> std::vector<int> bfs(const G& g, int start = 0)
{
    if (g.size() == 0)
    {
        return {};
    }
    if (start < 0 || start >= g.size())
    {
        throw std::out_of_range("非法起点");
    }
    std::vector<bool> seen(g.size());
    std::vector<int> out;
    std::queue<int> q;
    // 这个局部函数只负责搜索一个连通分量或可达区域
    auto component = [&](int s)
    {
        seen[s] = true;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            out.push_back(u);
            for (auto e : g.neighbors(u))
            {
                if (!seen[e.to])
                {
                    // 发现新顶点时立即标记，防止它被多个前驱重复加入队列
                    seen[e.to] = true;
                    q.push(e.to);
                }
            }
        }
    };
    // 先完成指定起点的搜索，再补充其他尚未访问的分量
    component(start);
    for (int u = 0; u < g.size(); ++u)
    {
        if (!seen[u])
        {
            component(u);
        }
    }
    return out;
}

// 深度优先搜索沿当前路径递归深入，无法继续时再回溯
template <class G> std::vector<int> dfs(const G& g, int start = 0)
{
    if (g.size() == 0)
    {
        return {};
    }
    if (start < 0 || start >= g.size())
    {
        throw std::out_of_range("非法起点");
    }
    std::vector<bool> seen(g.size());
    std::vector<int> out;
    // 递归函数先访问当前顶点，再依次处理未访问的邻居
    std::function<void(int)> visit = [&](int u)
    {
        seen[u] = true;
        out.push_back(u);
        for (auto e : g.neighbors(u))
        {
            if (!seen[e.to])
            {
                visit(e.to);
            }
        }
    };
    visit(start);
    for (int u = 0; u < g.size(); ++u)
    {
        if (!seen[u])
        {
            visit(u);
        }
    }
    return out;
}

// 保存已经输出的顶点顺序，以及是否完成全部顶点的拓扑排序
struct Topology
{
    std::vector<int> order;
    bool acyclic;
};

// Kahn 算法通过不断移除零入度顶点，完成拓扑排序并检测有向环
template <class G> Topology topological(const G& g)
{
    // degree 保存每个顶点尚未移除的入边数量
    std::vector<int> degree(g.size()), out;
    for (int u = 0; u < g.size(); ++u)
    {
        for (auto e : g.neighbors(u))
        {
            ++degree[e.to];
        }
    }
    // FIFO 队列保证邻接链表版本为 O(V+E)，初始顶点按编号入队
    std::queue<int> ready;
    for (int u = 0; u < g.size(); ++u)
    {
        if (degree[u] == 0)
        {
            ready.push(u);
        }
    }
    while (!ready.empty())
    {
        int u = ready.front();
        ready.pop();
        out.push_back(u);
        // 移除当前出边后，只有入度刚好变为零的顶点才加入队列
        for (auto e : g.neighbors(u))
        {
            if (--degree[e.to] == 0)
            {
                ready.push(e.to);
            }
        }
    }
    // 输出数量小于顶点总数时，剩余部分中必然存在有向环
    return {out, out.size() == size_t(g.size())};
}

// 使用足够大的有限值表示不可达，并为距离加边权预留空间
constexpr long long INF = std::numeric_limits<long long>::max() / 4;

// 最短路结果同时保存费用和前驱，便于恢复具体交换路线
struct Shortest
{
    std::vector<long long> distance;
    std::vector<int> previous;
};

// Dijkstra 使用小根堆选择当前暂定距离最小的顶点
template <class G> Shortest dijkstra(const G& g, int source)
{
    if (source < 0 || source >= g.size())
    {
        throw std::out_of_range("非法源点");
    }
    // 即使负权边不在源点可达区域，也明确拒绝不符合算法前提的图
    for (int u = 0; u < g.size(); ++u)
    {
        for (auto e : g.neighbors(u))
        {
            if (e.weight < 0)
            {
                throw std::invalid_argument("Dijkstra 不接受负权边");
            }
        }
    }
    Shortest out{std::vector<long long>(g.size(), INF), std::vector<int>(g.size(), -1)};
    // 堆中先按距离排序，距离相同时再按顶点编号排序
    using Item = std::pair<long long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> heap;
    out.distance[source] = 0;
    heap.push({0, source});
    while (!heap.empty())
    {
        auto [du, u] = heap.top();
        heap.pop();
        // 同一顶点可能有旧条目留在堆中，距离过期时直接跳过
        if (du != out.distance[u])
        {
            continue;
        }
        for (auto e : g.neighbors(u))
        {
            if (du + e.weight < out.distance[e.to])
            {
                // 找到更便宜的路线后，更新距离并记录这条路线的前驱
                out.distance[e.to] = du + e.weight;
                out.previous[e.to] = u;
                heap.push({out.distance[e.to], e.to});
            }
        }
    }
    return out;
}

// 从终点沿前驱链反向回溯，再翻转为从源点出发的路线
inline std::vector<int> shortestPath(const Shortest& s, int target)
{
    if (target < 0 || target >= int(s.distance.size()))
    {
        throw std::out_of_range("非法终点");
    }
    // 不可达的终点没有合法路径，返回空序列
    if (s.distance[target] == INF)
    {
        return {};
    }
    std::vector<int> out;
    for (int u = target; u != -1; u = s.previous[u])
    {
        out.push_back(u);
    }
    std::reverse(out.begin(), out.end());
    return out;
}
