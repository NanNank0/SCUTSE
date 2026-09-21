#include "Game.h"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <queue>
#include <limits>
#include <vector>
#include <memory>
#include "Pathfinding.h"
#include "Guard.h"


// 打开一个作弊/辅助窗口，用于计算在当前地图和守卫状态下，给定金币预算内
// 最优的守卫放置方案（最大化对路径上敌人的总伤害，同时尽量不把守卫放在敌人路径上）

void Game::showCheatWindow() {
    // 创建作弊窗口和 TGUI 界面
    int winW = 720, winH = 580;
    sf::RenderWindow cw(sf::VideoMode(sf::Vector2u(winW, winH)), "Cheat - Optimal Guard Placement");
    tgui::Gui gui(cw);
    try { tgui::Font ft("resources/wryh.ttf"); gui.setFont(ft); } catch (...) {}

    // 创建输入框用于设置金币预算
    auto goldInput = tgui::EditBox::create();
    goldInput->setPosition(20, 15); goldInput->setSize(100, 30);
    goldInput->setDefaultText("Gold"); goldInput->setText("30");
    goldInput->setTextSize(16);
    gui.add(goldInput);

    // 定义结果存储结构（内部使用）
    std::vector<CheatSuggestion> result;                // 存储每个守卫的建议（位置、类型、方向）
    int totalDmg = 0, goldUsed = 0, totalGuards = 0;
    bool calculated = false;
    int wastedGold = 0, wastedCount = 0;                // 记录因守卫被放置在路径上而“浪费”的金币

    // Calculate按钮执行最优计算
    auto calcBtn = tgui::Button::create();
    calcBtn->setPosition(140, 15); calcBtn->setSize(100, 30);
    calcBtn->setText("Calculate"); calcBtn->setTextSize(15);
    calcBtn->getRenderer()->setBackgroundColor(tgui::Color(180, 220, 180));
    calcBtn->onPress([&] {
int budget = std::max(1, tgui::String(goldInput->getText()).toInt());
        result.clear(); totalDmg = 0; goldUsed = 0; totalGuards = 0;
        wastedGold = 0; wastedCount = 0;

        // 使用Dijkstra算法根据权重矩阵计算最短路径
        auto findPathByWeight = [&](const std::vector<std::vector<int>>& w) {
            using Node = std::pair<int, std::pair<int,int>>;
            std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
            std::vector<std::vector<int>> d(m_n, std::vector<int>(m_n, INT_MAX));
            std::vector<std::vector<sf::Vector2i>> pr(m_n, std::vector<sf::Vector2i>(m_n, {-1,-1}));
            d[m_spawn.x][m_spawn.y] = 0;
            pq.push({0, {m_spawn.x, m_spawn.y}});
            int dr[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
            while (!pq.empty()) {
                auto [cd, pos] = pq.top(); pq.pop();
                auto [r, c] = pos;
                if (cd > d[r][c]) continue;
                if (r == m_defense.x && c == m_defense.y) break;
                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i], nc = c + dc[i];
                    if (nr < 0 || nr >= m_n || nc < 0 || nc >= m_n) continue;
                    int nd = cd + w[nr][nc];
                    if (nd < d[nr][nc]) { d[nr][nc] = nd; pr[nr][nc] = {r,c}; pq.push({nd, {nr,nc}}); }
                }
            }
            std::vector<sf::Vector2i> res;
            if (d[m_defense.x][m_defense.y] == INT_MAX) return res;
            sf::Vector2i cur = m_defense;
            while (cur.x != -1) { res.push_back(cur); if (cur == m_spawn) break; cur = pr[cur.x][cur.y]; }
            std::reverse(res.begin(), res.end());
            return res;
        };

        // 计算伤害：考虑已有守卫（type>0）以及新放置的守卫
        auto computeDamage = [&](const std::vector<CheatSuggestion>& newGs, const std::vector<sf::Vector2i>& p) {
            if (p.empty()) return 0;
            std::vector<std::vector<bool>> onP(m_n, std::vector<bool>(m_n, false));
            for (auto& cell : p) onP[cell.x][cell.y] = true;
            int dmg = 0;
            // 统计地图上已有守卫（不在路径上的）
            for (int r = 0; r < m_n; ++r) for (int c = 0; c < m_n; ++c) {
                if (!m_guards[r][c] || onP[r][c]) continue;
                Guard* g = m_guards[r][c].get();
                for (auto& pc : p) if (g->inRange({r,c}, pc)) dmg += g->getAttack();
            }
            // 统计新守卫（CheatSuggestion中type>0且不在路径上的）
            int wCosts2[6] = {0,3,3,4,5,1};
            for (auto& s : newGs) {
                if (s.type <= 0 || onP[s.row][s.col]) continue;
                auto tg = [&]() -> std::unique_ptr<Guard> {
                    switch (s.type) {
                        case 1: return std::make_unique<TankGuard>(s.dir);
                        case 2: return std::make_unique<SwordmanGuard>(s.dir);
                        case 3: return std::make_unique<ArcherGuard>(s.dir);
                        case 4: return std::make_unique<MageGuard>(s.dir);
                        case 5: return std::make_unique<PlaceholderGuard>(s.dir);
                        default: return nullptr;
                    }
                }();
                if (!tg) continue;
                for (auto& pc : p) if (tg->inRange({s.row, s.col}, pc)) dmg += tg->getAttack();
            }
            return dmg;
        };

        // 初始化权重矩阵（空格权重=1，守卫位置为守卫权重）
        auto weights = std::vector<std::vector<int>>(m_n, std::vector<int>(m_n, 1));
        for (int r = 0; r < m_n; ++r)
            for (int c = 0; c < m_n; ++c)
                if (m_guards[r][c]) weights[r][c] = m_guards[r][c]->getWeight();

        // 计算当前路径
        auto curPath = findPathByWeight(weights);
        if (curPath.empty()) { calculated = true; return; }
        auto occ = std::vector<std::vector<bool>>(m_n, std::vector<bool>(m_n, false));
        occ[m_spawn.x][m_spawn.y] = true; occ[m_defense.x][m_defense.y] = true;

        // 存储新添加的守卫（即算法推荐放置的）
        std::vector<CheatSuggestion> myNew;

        // 更新result列表（包含已有守卫和推荐守卫的状态）
        auto evaluateAll = [&]() {
            std::vector<std::vector<bool>> onP(m_n, std::vector<bool>(m_n, false));
            for (auto& cell : curPath) onP[cell.x][cell.y] = true;
            result.clear(); totalDmg = 0; totalGuards = 0; wastedGold = 0; wastedCount = 0;
            int costs2[6] = {0,3,3,4,5,1};
            for (auto& n : myNew) result.push_back(n);
            for (int r = 0; r < m_n; ++r) for (int c = 0; c < m_n; ++c) {
                if (!m_guards[r][c]) continue;
                occ[r][c] = true;
                Guard* g = m_guards[r][c].get();
                if (onP[r][c]) {
                    wastedGold += costs2[g->getCost()]; wastedCount++;
                    result.push_back({r, c, -1, g->getDirection()});
                } else {
                    int d = 0;
                    for (auto& pc : curPath) if (g->inRange({r,c}, pc)) d += g->getAttack();
                    totalGuards++; totalDmg += d;
                    result.push_back({r, c, -2, g->getDirection()});
                }
            }
            totalDmg = computeDamage(myNew, curPath);
        };
        evaluateAll();

        // 贪心选择最佳守卫（基于当前最优策略）
        int wCosts[6] = {0,3,3,4,5,1};
        int wWeights[6] = {0,10,4,2,1,2};
        while (goldUsed < budget) {
            int bestR = -1, bestC = -1, bestType = -1;
            Direction bestDir = Direction::Up;
            int bestScore = -1;

            for (int r = 0; r < m_n; ++r) {
                for (int c = 0; c < m_n; ++c) {
                    if (occ[r][c] || m_guards[r][c]) continue;
                    for (int t = 1; t <= 5; ++t) {
                        int cst = wCosts[t];
                        if (goldUsed + cst > budget) continue;
                        Direction dirs[4] = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
                        for (int di = 0; di < 4; ++di) {
                            Direction dir = dirs[di];
                            // 临时放置该守卫，看新路径
                            int oldW = weights[r][c];
                            weights[r][c] = wWeights[t];
                            auto newPath = findPathByWeight(weights);
                            weights[r][c] = oldW;
                            if (newPath.empty()) continue;

                            // 计算得分 = 总伤害 + 路径影响
                            auto allNew = myNew;
                            allNew.push_back({r, c, t, dir});

                            int td = computeDamage(allNew, newPath);
                            if (td > bestScore) {
                                bestScore = td; bestR = r; bestC = c; bestType = t; bestDir = dir;
                            }
                        }
                    }
                }
            }
            if (bestR < 0) break;

            // 放置该守卫，更新占用和权重
            occ[bestR][bestC] = true;
            weights[bestR][bestC] = wWeights[bestType];
            goldUsed += wCosts[bestType];
            totalGuards++;
            myNew.push_back({bestR, bestC, bestType, bestDir});

            // 更新路径和统计数据
            curPath = findPathByWeight(weights);
            evaluateAll();
        }
        // 最终重新计算总伤害（用于显示）
        totalDmg = computeDamage(myNew, curPath);
        calculated = true;    });
    gui.add(calcBtn);

	// Close按钮关闭作弊窗口
    auto closeBtn = tgui::Button::create();
    closeBtn->setPosition(260, 15); closeBtn->setSize(80, 30);
    closeBtn->setText("Close"); closeBtn->setTextSize(15);
    closeBtn->onPress([&]{ cw.close(); });
    gui.add(closeBtn);

    // 计算地图绘制参数
    int gridX = 30, gridY = 70, cSize = std::min(50, (winW - 300) / m_n);
    if (cSize > 45) cSize = 45;

    // 作弊窗口的主循环
    while (cw.isOpen()) {
        while (const auto oe = cw.pollEvent()) {
            if (oe->is<sf::Event::Closed>()) cw.close();
            gui.handleEvent(*oe);
        }
        cw.clear(sf::Color(40, 40, 45));

        // 网格绘制
        sf::RectangleShape cell(sf::Vector2f(cSize, cSize));
        cell.setOutlineThickness(1); cell.setOutlineColor(sf::Color(100,100,100));

        for (int r = 0; r < m_n; ++r) {
            for (int c = 0; c < m_n; ++c) {
                cell.setFillColor(sf::Color::Transparent);
                cell.setPosition(sf::Vector2f(gridX + c * cSize, gridY + r * cSize));
                cw.draw(cell);
            }
        }

        // 绘制守卫，颜色与字母
        sf::Color typeColors[6] = {sf::Color::Transparent, sf::Color(100,200,255),
            sf::Color(255,100,100), sf::Color(100,255,100), sf::Color(200,100,255), sf::Color(255,255,100)};
        char typeChars[6] = {' ', 'K', 'S', 'A', 'M', 'P'};

        for (auto& s : result) {
            sf::RectangleShape gr(sf::Vector2f(cSize, cSize));
            gr.setFillColor(typeColors[s.type]);
            gr.setOutlineThickness(2); gr.setOutlineColor(sf::Color::White);
            gr.setPosition(sf::Vector2f(gridX + s.col * cSize, gridY + s.row * cSize));
            cw.draw(gr);

			// 守卫类型字母绘制
            sf::Text gt(m_font, std::string(1, typeChars[s.type]), cSize * 0.6f);
            gt.setFillColor(sf::Color::Black);
            auto gb = gt.getLocalBounds();
            gt.setOrigin(sf::Vector2f(gb.size.x/2, gb.size.y/2));
            gt.setPosition(sf::Vector2f(gridX + s.col * cSize + cSize/2, gridY + s.row * cSize + cSize/2));
            cw.draw(gt);

            // 绘制守卫的方向箭头
            sf::ConvexShape arrow;
            arrow.setPointCount(3);
            arrow.setFillColor(sf::Color(255,255,255,200));
            float ax = gridX + s.col * cSize + cSize / 2.0f;
            float ay = gridY + s.row * cSize + cSize / 2.0f;
            float asz = cSize * 0.28f;
            switch (s.dir) {
                case Direction::Up:
                    arrow.setPoint(0, sf::Vector2f(ax, ay - asz));
                    arrow.setPoint(1, sf::Vector2f(ax - asz * 0.6f, ay + asz * 0.5f));
                    arrow.setPoint(2, sf::Vector2f(ax + asz * 0.6f, ay + asz * 0.5f));
                    break;
                case Direction::Down:
                    arrow.setPoint(0, sf::Vector2f(ax, ay + asz));
                    arrow.setPoint(1, sf::Vector2f(ax - asz * 0.6f, ay - asz * 0.5f));
                    arrow.setPoint(2, sf::Vector2f(ax + asz * 0.6f, ay - asz * 0.5f));
                    break;
                case Direction::Left:
                    arrow.setPoint(0, sf::Vector2f(ax - asz, ay));
                    arrow.setPoint(1, sf::Vector2f(ax + asz * 0.5f, ay - asz * 0.6f));
                    arrow.setPoint(2, sf::Vector2f(ax + asz * 0.5f, ay + asz * 0.6f));
                    break;
                case Direction::Right:
                    arrow.setPoint(0, sf::Vector2f(ax + asz, ay));
                    arrow.setPoint(1, sf::Vector2f(ax - asz * 0.5f, ay - asz * 0.6f));
                    arrow.setPoint(2, sf::Vector2f(ax - asz * 0.5f, ay + asz * 0.6f));
                    break;
            }
            cw.draw(arrow);

        }

        // 绘制进攻防守点
        sf::RectangleShape sr(sf::Vector2f(cSize, cSize));
        sr.setOutlineThickness(2); sr.setOutlineColor(sf::Color::White);
        sr.setFillColor(sf::Color::Red);
        sr.setPosition(sf::Vector2f(gridX + m_spawn.y * cSize, gridY + m_spawn.x * cSize));
        cw.draw(sr);
        sr.setFillColor(sf::Color::Blue);
        sr.setPosition(sf::Vector2f(gridX + m_defense.y * cSize, gridY + m_defense.x * cSize));
        cw.draw(sr);

        // 显示统计信息
        int infoX = gridX + m_n * cSize + 30;
        int iy = 70;
        sf::Text ttl(m_font, "=== Results ===", 16);
        ttl.setFillColor(sf::Color(200,255,200));
        ttl.setPosition(sf::Vector2f(infoX, iy)); cw.draw(ttl); iy += 28;

        auto drawInfo = [&](const std::string& lb, const std::string& vl, const sf::Color& vc) {
            sf::Text lt(m_font, lb, 14); lt.setFillColor(sf::Color(180,180,180));
            lt.setPosition(sf::Vector2f(infoX, iy)); cw.draw(lt);
            sf::Text vt(m_font, vl, 14); vt.setFillColor(vc);
            vt.setPosition(sf::Vector2f(infoX + 100, iy)); cw.draw(vt);
            iy += 22;
        };

        drawInfo("Map:", std::to_string(m_n)+"x"+std::to_string(m_n), sf::Color::White);
        drawInfo("Budget:", std::to_string(goldUsed)+"/"+tgui::String(goldInput->getText()).toStdString(), sf::Color::Yellow);
        drawInfo("Guards:", std::to_string(totalGuards), sf::Color::White);
        if (calculated) drawInfo("Total Dmg:", std::to_string(totalDmg), sf::Color(255,200,100));
        if (calculated && wastedCount > 0) drawInfo("Wasted:", std::to_string(wastedGold)+"g ("+std::to_string(wastedCount)+" guards)", sf::Color(255,80,80));

        // 显示图例
        if (calculated && totalGuards > 0) {
            iy += 10;
            sf::Text lg(m_font, "--- Legend ---", 14);
            lg.setFillColor(sf::Color(150,200,255));
            lg.setPosition(sf::Vector2f(infoX, iy)); cw.draw(lg); iy += 22;

            const char* typeNames[6] = {"", "Tank(K)", "Sword(S)", "Archer(A)", "Mage(M)", "Place(P)"};
            for (int t = 1; t <= 5; ++t) {
                int cnt = 0;
                for (auto& s : result) if (s.type == t) cnt++;
                if (cnt > 0) {
                    sf::Text lt(m_font, std::string(typeNames[t]) + " x" + std::to_string(cnt), 13);
                    lt.setFillColor(typeColors[t]);
                    lt.setPosition(sf::Vector2f(infoX + 10, iy)); cw.draw(lt); iy += 18;
                }
            }
        }

        gui.draw(); cw.display();
    }
}
