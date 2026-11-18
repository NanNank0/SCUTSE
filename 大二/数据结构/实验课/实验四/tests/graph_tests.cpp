/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#include <iostream>
#include <random>
#include "../common/graph_io.h"

inline void check(bool condition, const char* message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}

// 用固定例子与随机输入对照独立结果，检查算法的正常情况和边界情况
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    try
    {
        auto traversal = parseGraph("8 9 0\nA B C D E F G H\n0 1 1\n0 2 1\n1 3 1\n1 4 1\n2 4 1\n2 "
                                    "5 1\n3 6 1\n4 6 1\n5 6 1\n");
        auto m = traversal.build<MatrixGraph>();
        auto l = traversal.build<ListGraph>();
        check(bfs(m) == std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7}), "BFS expected order");
        check(dfs(m) == std::vector<int>({0, 1, 3, 6, 4, 2, 5, 7}), "DFS expected order");
        check(bfs(m) == bfs(l) && dfs(m) == dfs(l), "representation traversal mismatch");
        auto exchange = parseGraph("6 8 1\nS P V D G Z\n0 1 0\n0 2 50\n2 3 150\n1 3 300\n2 4 "
                                   "200\n1 4 350\n3 5 100\n4 5 200\n");
        auto answer = dijkstra(exchange.build<ListGraph>(), 0);
        check(answer.distance[5] == 300 &&
                  shortestPath(answer, 5) == std::vector<int>({0, 2, 3, 5}),
              "exchange answer");
        auto self = parseGraph("1 1 1\nA\n0 0 0\n");
        check(!topological(self.build<MatrixGraph>()).acyclic, "self loop");
        auto un = parseGraph("3 1 1\nS A T\n0 1 0\n");
        check(shortestPath(dijkstra(un.build<ListGraph>(), 0), 2).empty(), "unreachable");
        int rejected = 0;
        for (auto text : {"2 1 1 A B 0 2 1", "2 1 1 A B 0 1 -1", "2 2 0 A B 0 1 1 1 0 1",
                          "1 0 1 A extra", "2 0 1 A A"})
        {
            try
            {
                parseGraph(text);
            }
            catch (const std::exception&)
            {
                ++rejected;
            }
        }
        check(rejected == 5, "parser validation");
        ListGraph negative(2);
        negative.add(0, 1, -1);
        bool denied = false;
        try
        {
            dijkstra(negative, 0);
        }
        catch (const std::invalid_argument&)
        {
            denied = true;
        }
        check(denied, "negative weight check");
        std::mt19937 rng(20261118);
        for (int trial = 0; trial < 250; ++trial)
        {
            int n = 1 + rng() % 20;
            MatrixGraph a(n);
            ListGraph b(n);
            std::vector<std::tuple<int, int, int>> edges;
            for (int u = 0; u < n; ++u)
            {
                for (int v = 0; v < n; ++v)
                {
                    if (rng() % 7 == 0)
                    {
                        int w = rng() % 40;
                        a.add(u, v, w);
                        b.add(u, v, w);
                        edges.push_back({u, v, w});
                    }
                }
            }
            check(bfs(a) == bfs(b) && dfs(a) == dfs(b), "random traversal mismatch");
            auto ta = topological(a), tb = topological(b);
            check(ta.acyclic == tb.acyclic && ta.order == tb.order, "topology mismatch");
            // 独立的传递闭包作为判环参照，自环也被覆盖
            std::vector<std::vector<bool>> reach(n, std::vector<bool>(n));
            for (auto [u, v, w] : edges)
            {
                reach[u][v] = true;
            }
            for (int k = 0; k < n; ++k)
            {
                for (int u = 0; u < n; ++u)
                {
                    for (int v = 0; v < n; ++v)
                    {
                        reach[u][v] = reach[u][v] || (reach[u][k] && reach[k][v]);
                    }
                }
            }
            bool cycle = false;
            for (int u = 0; u < n; ++u)
            {
                cycle = cycle || reach[u][u];
            }
            check(ta.acyclic == !cycle, "topology vs closure");
            if (ta.acyclic)
            {
                std::vector<int> pos(n);
                for (int i = 0; i < n; ++i)
                {
                    pos[ta.order[i]] = i;
                }
                for (auto [u, v, w] : edges)
                {
                    check(pos[u] < pos[v], "invalid topology order");
                }
            }
            auto da = dijkstra(a, 0), db = dijkstra(b, 0);
            // Bellman-Ford 独立松弛过程，核对最短距离
            std::vector<long long> expected(n, INF);
            expected[0] = 0;
            for (int k = 1; k < n; ++k)
            {
                for (auto [u, v, w] : edges)
                {
                    if (expected[u] != INF)
                    {
                        expected[v] = std::min(expected[v], expected[u] + w);
                    }
                }
            }
            check(da.distance == expected && db.distance == expected, "Dijkstra vs Bellman-Ford");
            for (int target = 0; target < n; ++target)
            {
                auto path = shortestPath(db, target);
                if (expected[target] == INF)
                {
                    check(path.empty(), "unreachable path");
                    continue;
                }
                check(path.front() == 0 && path.back() == target, "path endpoints");
                long long cost = 0;
                for (size_t i = 1; i < path.size(); ++i)
                {
                    bool found = false;
                    for (auto e : b.neighbors(path[i - 1]))
                    {
                        if (e.to == path[i])
                        {
                            cost += e.weight;
                            found = true;
                        }
                    }
                    check(found, "path edge");
                }
                check(cost == expected[target], "path cost");
            }
        }
        std::cout << "PASS: graph examples, invalid input, zero weights, self loops, unreachable, "
                     "250 random graphs vs independent references\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
