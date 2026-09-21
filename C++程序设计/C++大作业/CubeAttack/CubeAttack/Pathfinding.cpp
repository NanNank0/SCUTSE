#include "Pathfinding.h"
#include <queue>
#include <limits>
#include <algorithm>


/*
// 这里本来是想写一个让守卫动一步判断一次
// 然后随着敌人运动逐渐去摧毁守卫更新地图然后重新算路径的
// 然后发现这样写每个敌人都要判断一次有点太麻烦了
// 最重要的一点： 我不会。
*/

/*
// 这个寻路逻辑是：
// 1. 先计算出从spawn到defense的最优路径，考虑守卫的权重（Dijkstra）
// 2. 然后没了，这条路径有守卫就直接摧毁。
*/

std::vector<sf::Vector2i> findOptimalPath(
    int n,
    const sf::Vector2i& spawn,
    const sf::Vector2i& defense,
    const std::vector<std::vector<std::unique_ptr<Guard>>>& guards)
{
    // 4个移动方向
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    // 距离表
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, std::numeric_limits<int>::max()));

    // 回溯节点
    std::vector<std::vector<sf::Vector2i>> parent(n, std::vector<sf::Vector2i>(n, {-1, -1}));

    // 优先队列
    using Node = std::pair<int, std::pair<int, int>>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    dist[spawn.x][spawn.y] = 0;
    pq.push({0, {spawn.x, spawn.y}});

    while (!pq.empty()) {
        auto [d, pos] = pq.top();
        auto [r, c] = pos;
        pq.pop();

        if (d > dist[r][c]) continue;
        if (r == defense.x && c == defense.y) break;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;

			// 网格权重：空为1，守卫权重为guards[nr][nc]->getWeight()
            int weight = 1;
            if (guards[nr][nc] != nullptr) {
                weight = guards[nr][nc]->getWeight();
            }

            int nd = d + weight;
            if (nd < dist[nr][nc]) {
                dist[nr][nc] = nd;
                parent[nr][nc] = {r, c};
                pq.push({nd, {nr, nc}});
            }
        }
    }

    // 然后回溯
    std::vector<sf::Vector2i> path;
    if (dist[defense.x][defense.y] == std::numeric_limits<int>::max()) {
        return path;
    }

    sf::Vector2i current = defense;
    while (current.x != -1 && current.y != -1) {
        path.push_back(current);
        if (current.x == spawn.x && current.y == spawn.y) break;
        current = parent[current.x][current.y];
    }

    std::reverse(path.begin(), path.end());
    return path;
}
