#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <deque>
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
    void placeGuard(int row, int col, int type, Direction dir);
    void upgradeGuard(int row, int col, bool upgradeAttack);
    void spawnEnemiesForRound();
    void findOptimalPath();
    void destroyGuardsOnPath();
    void applyGuardAttacks();
    void checkGameOver();
    void drawGrid(int cellSize, int margin);
    void drawEnemies(int cellSize, int margin);
    void drawRightPanel();
    void drawAttackRange();
    void log(const std::string& text, const sf::Color& color = sf::Color::White);
    void showGameOverWindow();
    void showCheatWindow();
    void updateUpgradeButtonVisibility();
    struct LogEntry { std::string text; sf::Color color; };
    struct CheatSuggestion { int row; int col; int type; Direction dir; };

    int m_n; int m_totalRounds; int m_defenseHp; int m_currentRound; int m_gold;
    bool m_removeMode; bool m_gameOver; bool m_gameWon;
    sf::Vector2i m_spawn; sf::Vector2i m_defense; sf::Vector2i m_selectedGuardPos;
    std::vector<std::vector<std::unique_ptr<Guard>>> m_guards;
    Map m_map; sf::RenderWindow m_window; tgui::Gui m_gui;
    std::vector<std::vector<int>> m_dist;
    std::vector<Enemy> m_pendingEnemies; std::vector<Enemy> m_activeEnemies;
    sf::Clock m_moveClock; float m_moveInterval = 0.5f;
    std::vector<sf::Vector2i> m_currentPath;
    std::deque<LogEntry> m_log; int m_logScroll;
    static constexpr int MAX_LOG_ENTRIES = 100;
    sf::Texture m_winTexture; sf::Texture m_loseTexture;
    sf::SoundBuffer m_winSoundBuffer; sf::SoundBuffer m_loseSoundBuffer;

    void setupUI(); void updateWindowTitle(); void updateTypeButtonColors(); void updateDirButtonColors();
    sf::Vector2i getCellFromMouse(int mx, int my, int cs, int mg) const;
    int m_selectedType; Direction m_selectedDirection;
    std::vector<tgui::Button::Ptr> m_typeBtns; std::vector<tgui::Button::Ptr> m_dirBtns;
    tgui::Button::Ptr m_endTurnBtn; tgui::Button::Ptr m_upgAtkBtn; tgui::Button::Ptr m_upgWgtBtn;
    enum class Phase { Defense, Attack }; Phase m_phase;
    sf::Font m_font;
    sf::Texture m_tankTexture; sf::Texture m_swordmanTexture; sf::Texture m_archerTexture;
    sf::Texture m_mageTexture; sf::Texture m_placeholderTexture;
    static constexpr int CELL_SIZE = 60; static constexpr int MARGIN = 80;
    static constexpr int UI_HEIGHT = 120; static constexpr int RIGHT_PANEL_WIDTH = 260;
    static constexpr int RIGHT_PANEL_GAP = 15;
};
