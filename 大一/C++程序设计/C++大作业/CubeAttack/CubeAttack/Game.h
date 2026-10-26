#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Map.h"
#include "Guard.h"
#include "Enemy.h"
#include "Pathfinding.h"
#include <SFML/Graphics/Texture.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <queue>
#include <limits>

class Game {
public:
    Game(int n, int totalRounds, int defenseHp);
    void run();

    void switchPhase();
    void startAttackPhase();
    void endAttackPhase();
private:

    void handleEvents();
    void update();
    void render();

    // 守卫相关（放置与升级）
    void placeGuard(int row, int col, int type, Direction dir);
    void upgradeGuard(int row, int col, bool upgradeAttack);

	// 敌人相关
    void spawnEnemiesForRound();
    void findOptimalPath();
    void destroyGuardsOnPath();
    void applyGuardAttacks();
    void checkGameOver();

    // 守卫/敌人绘制
    void drawGrid(int cellSize, int margin);
    void drawEnemies(int cellSize, int margin);

    // 成员
    int m_n;
    int m_totalRounds;
    int m_defenseHp;
    int m_currentRound;
    int m_gold;
    bool m_removeMode;
    sf::Vector2i m_spawn;
    sf::Vector2i m_defense;
    std::vector<std::vector<std::unique_ptr<Guard>>> m_guards;
    Map m_map;
    sf::RenderWindow m_window;
    tgui::Gui m_gui;
    std::vector<std::vector<int>> m_dist;
    std::vector<Enemy> m_pendingEnemies;   // 敌人未生成
    std::vector<Enemy> m_activeEnemies;    // 生成的敌人
    sf::Clock m_moveClock;
    float m_moveInterval = 0.5f;

    // 寻路
    std::vector<sf::Vector2i> m_currentPath;

    // UI
    void setupUI();
    void updateWindowTitle();
    void updateTypeButtonColors();
    void updateDirButtonColors();
    sf::Vector2i getCellFromMouse(int mouseX, int mouseY, int cellSize, int margin) const;
    int m_selectedType;
    Direction m_selectedDirection;
    std::vector<tgui::Button::Ptr> m_typeBtns;
    std::vector<tgui::Button::Ptr> m_dirBtns;
    tgui::Button::Ptr m_endTurnBtn;

    // 游戏阶段
    enum class Phase { Defense, Attack };
    Phase m_phase;

    // 字体
    sf::Font m_font;

    // 纹理
    sf::Texture m_tankTexture;
    sf::Texture m_swordmanTexture;
    sf::Texture m_archerTexture;
    sf::Texture m_mageTexture;
    sf::Texture m_placeholderTexture;

    // 基础UI相关数据
    static constexpr int CELL_SIZE = 60;
    static constexpr int MARGIN = 80;
    static constexpr int UI_HEIGHT = 120;
};

