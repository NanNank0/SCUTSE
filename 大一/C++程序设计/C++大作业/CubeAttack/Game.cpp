#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

Game::Game(int n, int totalRounds, int defenseHp)
    : m_n(n), m_totalRounds(totalRounds), m_defenseHp(defenseHp),
    m_currentRound(1), m_gold(30), m_map(n), m_phase(Phase::Defense),
    m_selectedType(0), m_selectedDirection(Direction::Up), m_removeMode(false),
    m_gameOver(false), m_gameWon(false),
    m_selectedGuardPos(-1, -1),
    m_logScroll(0),
    m_window(sf::VideoMode(sf::Vector2u(
        MARGIN + n * CELL_SIZE + RIGHT_PANEL_GAP + RIGHT_PANEL_WIDTH + 15,
        n * CELL_SIZE + MARGIN + UI_HEIGHT)),
        "Cube Attack - By NanNank0"),
    m_gui(m_window)
{
    m_dist.resize(n, std::vector<int>(n, 0));

    int minDist = std::max(3, n / 3);
    m_map.generateSpawnAndDefense(minDist);
    m_spawn = m_map.getSpawnPos();
    m_defense = m_map.getDefensePos();

    m_guards.resize(n);
    for (auto& row : m_guards) {
        row.resize(n);
    }

    // 这个SFML的字体加载问题好大，用了好多方法都没法正常加载
    // 我这里是直接用windos自带的微软雅黑
    try {
        m_font = sf::Font("resources/wryh.ttf");
    } catch (const std::exception& e) {
        std::cerr << "Failed to load font: " << e.what() << "\n";
    }
    // 加载贴图纹理
    auto loadTexture = [&](sf::Texture& tex, const std::string& path) {
        if (!tex.loadFromFile(path)) {
            std::cerr << "Failed to load " << path << "\n";
        }
    };
    loadTexture(m_tankTexture, "resources/tank.png");
    loadTexture(m_swordmanTexture, "resources/swordman.png");
    loadTexture(m_archerTexture, "resources/archer.png");
    loadTexture(m_mageTexture, "resources/mage.png");
    loadTexture(m_placeholderTexture, "resources/placeholder.png");

    // 胜利失败图片
	loadTexture(m_winTexture, "resources/win.png");
	loadTexture(m_loseTexture, "resources/lose.png");

    // 结束播放音效呜呼呼
    if (!m_winSoundBuffer.loadFromFile("resources/win.mp3")) {
        std::cerr << "Failed win.mp3\n";
    }
    if (!m_loseSoundBuffer.loadFromFile("resources/lose.wav")) {
        std::cerr << "Failed lose.wav\n";
    }

    log("Game init: " + std::to_string(n) + "x" + std::to_string(n));
    log("Spawn: (" + std::to_string(m_spawn.x) + "," + std::to_string(m_spawn.y) + ") Defense: (" + std::to_string(m_defense.x) + "," + std::to_string(m_defense.y) + ")");
    log("Round 1 - Defense Phase");

    setupUI();
    updateWindowTitle();
}

void Game::setupUI() {
    int mapBottom = MARGIN + m_n * CELL_SIZE;
    int buttonY = mapBottom + 10;
    int typeBtnWidth = 70;
    int typeBtnHeight = 30;
	// 下面是UI的布局，第一行是守卫类型按钮，第二行是方向按钮，第三行是移除按钮和结束回合按钮
    // 第一行：守卫类型
    std::vector<std::string> typeNames = { "Tank", "Sword", "Archer", "Mage", "Place" };
    int totalTypeWidth = typeBtnWidth * static_cast<int>(typeNames.size()) + 10 * (static_cast<int>(typeNames.size()) - 1);
    int startX = (static_cast<int>(m_window.getSize().x) - totalTypeWidth) / 2;

    for (size_t i = 0; i < typeNames.size(); ++i) {
        auto btn = tgui::Button::create();
        btn->setPosition(startX + static_cast<int>(i) * (typeBtnWidth + 10), buttonY);
        btn->setSize(typeBtnWidth, typeBtnHeight);
        btn->setText(typeNames[i]);
        btn->setTextSize(14);
        btn->getRenderer()->setBackgroundColor(tgui::Color(220, 220, 220));
        int typeId = static_cast<int>(i) + 1;

        btn->onPress([this, typeId] {
            if (m_selectedType == typeId) {
                m_selectedType = 0;
            } else {
                m_selectedType = typeId;
            }
            updateTypeButtonColors();
        });
        m_gui.add(btn);
        m_typeBtns.push_back(btn);
    }

    // 第二行：方向按钮
    std::vector<std::string> dirNames = { "Up", "Down", "Left", "Right" };
    std::vector<Direction> dirs = { Direction::Up, Direction::Down, Direction::Left, Direction::Right };
    int dirBtnWidth = 60;
    int dirBtnHeight = 30;
    int totalDirWidth = dirBtnWidth * static_cast<int>(dirNames.size()) + 10 * (static_cast<int>(dirNames.size()) - 1);
    startX = (static_cast<int>(m_window.getSize().x) - totalDirWidth) / 2;
    int dirY = buttonY + typeBtnHeight + 10;

    for (size_t i = 0; i < dirNames.size(); ++i) {
        auto btn = tgui::Button::create();
        btn->setPosition(startX + static_cast<int>(i) * (dirBtnWidth + 10), dirY);
        btn->setSize(dirBtnWidth, dirBtnHeight);
        btn->setText(dirNames[i]);
        btn->setTextSize(14);
        btn->getRenderer()->setBackgroundColor(tgui::Color(220, 220, 220));
        Direction dir = dirs[i];

        btn->onPress([this, btn, dir] {
            m_selectedDirection = dir;
            updateDirButtonColors();
        });
        m_gui.add(btn);
        m_dirBtns.push_back(btn);
    }

    // 第三行：移除守卫与结束回合
    int funcY = dirY + dirBtnHeight + 10;
    int btnSpacing = 20;
    int removeBtnWidth = 90;
    int removeBtnX = (static_cast<int>(m_window.getSize().x) - (removeBtnWidth * 2 + btnSpacing)) / 2;

    auto removeBtn = tgui::Button::create();
    removeBtn->setPosition(removeBtnX, funcY);
    removeBtn->setSize(removeBtnWidth, 30);
    removeBtn->setText("Remove");
    removeBtn->setTextSize(14);
    removeBtn->getRenderer()->setBackgroundColor(tgui::Color(220, 220, 220));
    removeBtn->onPress([this, removeBtn] {
        m_removeMode = !m_removeMode;
        if (m_removeMode) {
            removeBtn->getRenderer()->setBackgroundColor(tgui::Color::Red);
            removeBtn->setText("Remove (ON)");
        } else {
            removeBtn->getRenderer()->setBackgroundColor(tgui::Color(220, 220, 220));
            removeBtn->setText("Remove");
        }
    });
    m_gui.add(removeBtn);

    m_endTurnBtn = tgui::Button::create();
    m_endTurnBtn->setPosition(removeBtnX + removeBtnWidth + btnSpacing, funcY);
    m_endTurnBtn->setSize(removeBtnWidth, 30);
    m_endTurnBtn->setText("End Turn");
    m_endTurnBtn->setTextSize(14);
    m_endTurnBtn->getRenderer()->setBackgroundColor(tgui::Color(100, 200, 100));
    m_endTurnBtn->onPress([this] {
        if (m_phase == Phase::Defense) {
            switchPhase();
        }
    });
    m_gui.add(m_endTurnBtn);
    auto cheatBtn = tgui::Button::create();
    cheatBtn->setPosition(10, funcY);
    cheatBtn->setSize(60, 30);
    cheatBtn->setText("Cheat");
    cheatBtn->setTextSize(13);
    cheatBtn->getRenderer()->setBackgroundColor(tgui::Color(255,200,200));
    cheatBtn->onPress([this] { showCheatWindow(); });
    m_gui.add(cheatBtn);

    // 计算右侧面板的升级按钮位置
    int rpX = MARGIN + m_n * CELL_SIZE + RIGHT_PANEL_GAP;
    int rpH = m_n * CELL_SIZE;
    int lh = std::max(80, (int)(rpH * 0.55f));
    int ih = rpH - lh - 10;
    if (ih < 155) { ih = 155; lh = rpH - ih - 10; }
    int iT = MARGIN + lh + 5;

    m_upgAtkBtn = tgui::Button::create();
    m_upgAtkBtn->setPosition(rpX + 15, iT + ih - 34);
    m_upgAtkBtn->setSize(105, 28);
    m_upgAtkBtn->setText("Upg Attack (1g)");
    m_upgAtkBtn->setTextSize(13);
    m_upgAtkBtn->getRenderer()->setBackgroundColor(tgui::Color(180, 180, 255));
    m_upgAtkBtn->onPress([this] {
        if (m_selectedGuardPos.x >= 0 && m_phase == Phase::Defense)
            upgradeGuard(m_selectedGuardPos.x, m_selectedGuardPos.y, true);
    });
    m_upgAtkBtn->setVisible(false);
    m_gui.add(m_upgAtkBtn);

    m_upgWgtBtn = tgui::Button::create();
    m_upgWgtBtn->setPosition(rpX + 130, iT + ih - 34);
    m_upgWgtBtn->setSize(110, 28);
    m_upgWgtBtn->setText("Upg Weight (1g)");
    m_upgWgtBtn->setTextSize(13);
    m_upgWgtBtn->getRenderer()->setBackgroundColor(tgui::Color(255, 200, 150));
    m_upgWgtBtn->onPress([this] {
        if (m_selectedGuardPos.x >= 0 && m_phase == Phase::Defense)
            upgradeGuard(m_selectedGuardPos.x, m_selectedGuardPos.y, false);
    });
    m_upgWgtBtn->setVisible(false);
    m_gui.add(m_upgWgtBtn);
}

void Game::updateTypeButtonColors() {
    for (size_t i = 0; i < m_typeBtns.size(); ++i) {
        int typeId = static_cast<int>(i) + 1;
        if (typeId == m_selectedType) {
            m_typeBtns[i]->getRenderer()->setBackgroundColor(tgui::Color::Green);
        } else {
            m_typeBtns[i]->getRenderer()->setBackgroundColor(tgui::Color(220, 220, 220));
        }
    }
}

void Game::updateDirButtonColors() {
    for (size_t i = 0; i < m_dirBtns.size(); ++i) {
        Direction dirs[] = { Direction::Up, Direction::Down, Direction::Left, Direction::Right };
        if (dirs[i] == m_selectedDirection) {
            m_dirBtns[i]->getRenderer()->setBackgroundColor(tgui::Color::Green);
        } else {
            m_dirBtns[i]->getRenderer()->setBackgroundColor(tgui::Color(220, 220, 220));
        }
    }
}


void Game::log(const std::string& text, const sf::Color& color) {
    m_log.push_back({ text, color });
    if (m_log.size() > MAX_LOG_ENTRIES) {
        m_log.pop_front();
    }
    m_logScroll = 0;
}

// 游戏主循环
void Game::run() {
    while (m_window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    while (const auto optEvent = m_window.pollEvent()) {
        m_gui.handleEvent(*optEvent);

        const auto& event = *optEvent;
        if (event.is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (m_phase != Phase::Defense) return;

            sf::Vector2i cell = getCellFromMouse(mousePressed->position.x, mousePressed->position.y, CELL_SIZE, MARGIN);
            if (cell.x == -1) {
                if (!m_removeMode) {
                    m_selectedGuardPos = {-1, -1};
                    updateUpgradeButtonVisibility();
                }
                return;
            }

            if (m_removeMode) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    if (m_guards[cell.x][cell.y] != nullptr) {
                        m_guards[cell.x][cell.y].reset();
                        log("Guard removed");
                        if (m_selectedGuardPos == cell) {
                            m_selectedGuardPos = {-1, -1};
                            updateUpgradeButtonVisibility();
                        }
                    } else {
                        log("No guard here");
                    }
                }
                return;
            }

            if (mousePressed->button == sf::Mouse::Button::Left) {
                if (m_guards[cell.x][cell.y] != nullptr) {
                    if (m_selectedGuardPos == cell) {
                        m_selectedGuardPos = {-1, -1};
                    } else {
                        m_selectedGuardPos = cell;
                        Guard* g = m_guards[cell.x][cell.y].get();
                        std::string dirStr;
                        switch (g->getDirection()) {
                            case Direction::Up: dirStr = "U"; break;
                            case Direction::Down: dirStr = "D"; break;
                            case Direction::Left: dirStr = "L"; break;
                            case Direction::Right: dirStr = "R"; break;
                        }
                        log("Selected " + g->getName() + " A:" + std::to_string(g->getAttack()) + " W:" + std::to_string(g->getWeight()) + " D:" + dirStr);
                    }
                    updateUpgradeButtonVisibility();
                    m_selectedType = 0;
                    updateTypeButtonColors();
                } else if (m_map.getCellType(cell.x, cell.y) == CellType::Empty) {
                    m_selectedGuardPos = {-1, -1};
                    updateUpgradeButtonVisibility();
                    if (m_selectedType == 0) {
                        log("Select a guard type first");
                    } else {
                        placeGuard(cell.x, cell.y, m_selectedType, m_selectedDirection);
                    }
                } else {
                    log("Cannot place on spawn/defense point");
                    m_selectedGuardPos = {-1, -1};
                    updateUpgradeButtonVisibility();
                }
            }
        } else if (const auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
            int rpX = MARGIN + m_n * CELL_SIZE + RIGHT_PANEL_GAP;
            if (wheelScrolled->position.x >= rpX && wheelScrolled->position.x < rpX + RIGHT_PANEL_WIDTH) {
                int maxScroll = std::max(0, (int)m_log.size() - 6);
                m_logScroll = std::clamp(m_logScroll - (int)wheelScrolled->delta, 0, maxScroll);
            }
        } else if (const auto* keyPressed = optEvent->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                if (m_phase == Phase::Defense) {
                    switchPhase();
                }
            }
        }
    }
}

void Game::update() {
    if (m_phase != Phase::Attack) return;

    if (m_moveClock.getElapsedTime().asSeconds() < m_moveInterval) return;
    m_moveClock.restart();

    for (int i = static_cast<int>(m_activeEnemies.size()) - 1; i >= 0; --i) {
        int newIdx = m_activeEnemies[i].getPathIndex() + 1;

        if (newIdx >= static_cast<int>(m_currentPath.size())) {
            // 怪物到达防御点
            m_defenseHp--;
            log("Enemy reached defense! HP left: " + std::to_string(m_defenseHp), sf::Color::Red);
            m_activeEnemies.erase(m_activeEnemies.begin() + i);
            updateWindowTitle();
            continue;
        }

        m_activeEnemies[i].setPathIndex(newIdx);
        m_activeEnemies[i].setPos(m_currentPath[newIdx]);
    }

    // 守卫攻击
    applyGuardAttacks();

    // 进攻点没有单位 -> 生成敌人
    if (!m_pendingEnemies.empty()) {
        bool spawnOccupied = false;
        for (const auto& e : m_activeEnemies) {
            if (e.getPos() == m_spawn) {
                spawnOccupied = true;
                break;
            }
        }
        if (!spawnOccupied) {
            Enemy next = m_pendingEnemies.front();
            m_pendingEnemies.erase(m_pendingEnemies.begin());
            next.setPos(m_spawn);
            next.setPathIndex(0);
            m_activeEnemies.push_back(next);
            std::cout << "Enemy spawned. Remaining: " << m_pendingEnemies.size()
                      << " Active: " << m_activeEnemies.size() << "\n";
        }
    }

    // 进攻阶段结束
    if (m_activeEnemies.empty() && m_pendingEnemies.empty()) {
        endAttackPhase();
    }

	// 防御点血量为0 -> 游戏结束
    if (m_defenseHp <= 0) {
        checkGameOver();
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    drawGrid(CELL_SIZE, MARGIN);

    // 进攻点红色 防守点蓝色
    sf::RectangleShape specialRect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    specialRect.setOutlineThickness(1);
    specialRect.setOutlineColor(sf::Color::White);

    specialRect.setFillColor(sf::Color::Red);
    specialRect.setPosition(sf::Vector2f(MARGIN + m_spawn.y * CELL_SIZE, MARGIN + m_spawn.x * CELL_SIZE));
    m_window.draw(specialRect);

    specialRect.setFillColor(sf::Color::Blue);
    specialRect.setPosition(sf::Vector2f(MARGIN + m_defense.y * CELL_SIZE, MARGIN + m_defense.x * CELL_SIZE));
    m_window.draw(specialRect);

    // 守卫绘制
    for (int row = 0; row < m_n; ++row) {
        for (int col = 0; col < m_n; ++col) {
            if (m_guards[row][col] != nullptr) {
                m_window.draw(m_guards[row][col]->getSprite());
            }
        }
    }

    // 绘制攻击范围高亮（在守卫下方）
    drawAttackRange();

    // 敌人绘制
    drawEnemies(CELL_SIZE, MARGIN);

    // 绘制右侧面板（日志 + 属性）
    drawRightPanel();

    // UI绘制
    m_gui.draw();
    m_window.display();
}

void Game::drawEnemies(int cellSize, int margin) {
    for (const auto& enemy : m_activeEnemies) {
        sf::Vector2i pos = enemy.getPos();
        float cx = margin + pos.y * cellSize + cellSize / 2.0f;
        float cy = margin + pos.x * cellSize + cellSize / 2.0f;

        // 懒得找敌人贴图了就拿红色圈圈了
        sf::CircleShape circle(cellSize * 0.35f);
        circle.setFillColor(sf::Color(200, 50, 50));
        circle.setOutlineThickness(2);
        circle.setOutlineColor(sf::Color::Red);
        circle.setPosition(sf::Vector2f(cx - cellSize * 0.35f, cy - cellSize * 0.35f));
        m_window.draw(circle);

        // 直接把敌人血量显示在敌人圆圈的最中间
        sf::Text hpText(m_font);
        hpText.setString(std::to_string(enemy.getHp()));
        hpText.setCharacterSize(16);
        hpText.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = hpText.getLocalBounds();
        hpText.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
        hpText.setPosition(sf::Vector2f(cx, cy - 2));
        m_window.draw(hpText);
    }
}

// Gruad放置 ：判断类型，金币要够，放置点一定是空的，贴图和旋转
void Game::placeGuard(int row, int col, int type, Direction dir) {
    if (m_guards[row][col] != nullptr) {
        log("Guard already here");
        return;
    }
    if (m_map.getCellType(row, col) != CellType::Empty) {
        log("Cannot place on spawn/defense point");
        return;
    }

    int cost = 0;
    const sf::Texture* textureToUse = nullptr;
    std::unique_ptr<Guard> guard;

    switch (type) {
    case 1:
        guard = std::make_unique<TankGuard>(dir);
        cost = 3;
        textureToUse = &m_tankTexture;
        break;
    case 2:
        guard = std::make_unique<SwordmanGuard>(dir);
        cost = 3;
        textureToUse = &m_swordmanTexture;
        break;
    case 3:
        guard = std::make_unique<ArcherGuard>(dir);
        cost = 4;
        textureToUse = &m_archerTexture;
        break;
    case 4:
        guard = std::make_unique<MageGuard>(dir);
        cost = 5;
        textureToUse = &m_mageTexture;
        break;
    case 5:
        guard = std::make_unique<PlaceholderGuard>(dir);
        cost = 1;
        textureToUse = &m_placeholderTexture;
        break;
    default:
        return;
    }

    // 金币扣除逻辑
    if (m_gold < cost) {
        log("Need " + std::to_string(cost) + " gold");
        return;
    }
    m_gold -= cost;

	// 放置坐标计算，贴图设置，旋转
    float left = MARGIN + col * CELL_SIZE;
    float top = MARGIN + row * CELL_SIZE;

    guard->setupSprite(*textureToUse, sf::Vector2f(left + CELL_SIZE / 2.0f, top + CELL_SIZE / 2.0f));

    sf::FloatRect bounds = guard->getSprite().getLocalBounds();
    float texW = bounds.size.x;
    float texH = bounds.size.y;

	// 设置旋转中心为贴图中心
    guard->getSprite().setOrigin(sf::Vector2f(texW / 2.0f, texH / 2.0f));

    // 根据角度旋转
    switch (dir) {
    case Direction::Right:
        guard->getSprite().setRotation(sf::degrees(0));
        break;
    case Direction::Down:
        guard->getSprite().setRotation(sf::degrees(90));
        break;
    case Direction::Left:
        guard->getSprite().setRotation(sf::degrees(180));
        break;
    case Direction::Up:
        guard->getSprite().setRotation(sf::degrees(270));
        break;
    }// 这里以后如果要添加守卫的时候不想改这里就把贴图自己旋转一下哈哈

    m_guards[row][col] = std::move(guard);
    log("Guard placed. Gold left: " + std::to_string(m_gold));
    updateWindowTitle();
}

// 守卫升级逻辑
void Game::upgradeGuard(int row, int col, bool upgradeAttack) {
    if (m_guards[row][col] == nullptr) {
        log("No guard to upgrade");
        return;
    }
    if (m_gold < 1) {
        log("Need 1 gold to upgrade");
        return;
    }
    Guard* guard = m_guards[row][col].get();
    if (upgradeAttack) {
        if (guard->canUpgradeAttack()) {
            guard->upgradeAttack();
            m_gold -= 1;
            log("Attack upgraded to " + std::to_string(guard->getAttack()));
        } else {
            log("Attack already at max");
        }
    } else {
        guard->upgradeWeight();
        m_gold -= 1;
        log("Weight upgraded to " + std::to_string(guard->getWeight()));
    }
    updateWindowTitle();
}

// 网格绘制
void Game::drawGrid(int cellSize, int margin) {
    sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::White);
    rect.setFillColor(sf::Color::Transparent);
    for (int row = 0; row < m_n; ++row) {
        for (int col = 0; col < m_n; ++col) {
            rect.setPosition(sf::Vector2f(margin + col * cellSize, margin + row * cellSize));
            m_window.draw(rect);
        }
    }
}

// 更新窗口标题，显示当前回合数、金币、生命值、阶段和移除模式状态
void Game::updateWindowTitle() {
    std::string mode = m_removeMode ? " [REMOVE MODE]" : "";
    std::string title = "Turn " + std::to_string(m_currentRound) +
        "  Gold:" + std::to_string(m_gold) +
        "  Life:" + std::to_string(m_defenseHp) +
        "  Phase:" + (m_phase == Phase::Defense ? "Defense" : "Attack") + mode;
    m_window.setTitle(title);
}

// 判断鼠标位置是哪个网格
sf::Vector2i Game::getCellFromMouse(int mouseX, int mouseY, int cellSize, int margin) const {
    int col = (mouseX - margin) / cellSize;
    int row = (mouseY - margin) / cellSize;
    if (row >= 0 && row < m_n && col >= 0 && col < m_n) {
        return { row, col };
    }
    return { -1, -1 };
}

// 切换阶段
void Game::switchPhase() {
    if (m_phase == Phase::Defense) {
        m_phase = Phase::Attack;
        log(">>> Attack Phase <<<", sf::Color(255,200,100));
        m_endTurnBtn->setVisible(false);
        startAttackPhase();
    } else {
        m_phase = Phase::Defense;
        m_currentRound++;
        log(">>> Round " + std::to_string(m_currentRound) + " Defense <<<", sf::Color(150,200,255));
        m_endTurnBtn->setVisible(true);
        updateWindowTitle();
    }
}

// 进攻阶段
void Game::startAttackPhase() {
    // 寻找最优路径
    findOptimalPath();
    if (m_currentPath.empty()) {
        log("No path found! Skipping attack.", sf::Color::Red);
        endAttackPhase();
        return;
    }
    log("Path found: " + std::to_string(m_currentPath.size()) + " steps");

    // 直接摧毁路径上所有守卫
    destroyGuardsOnPath();

    // 召唤敌人
    spawnEnemiesForRound();

    if (!m_pendingEnemies.empty()) {
        Enemy first = m_pendingEnemies.front();
        m_pendingEnemies.erase(m_pendingEnemies.begin());
        first.setPos(m_spawn);
        first.setPathIndex(0);
        m_activeEnemies.push_back(first);
        log("First enemy spawned");
    }

    // 4. 移动时钟
    m_moveClock.restart();
    updateWindowTitle();
}

void Game::endAttackPhase() {
    log(">>> Attack Phase Ended <<<", sf::Color(150,200,255));

    if (m_defenseHp <= 0) {
        checkGameOver();
        return;
    }

    if (m_currentRound >= m_totalRounds) {
        m_gameOver = true;
        m_gameWon = true;
        m_window.setTitle("You Win! - Cube Attack");
        log("VICTORY! All rounds survived!", sf::Color(255,255,100));
        showGameOverWindow();
        return;
    }

    m_phase = Phase::Defense;
    m_currentRound++;
    m_activeEnemies.clear();
    m_pendingEnemies.clear();
    m_currentPath.clear();
    m_endTurnBtn->setVisible(true);
    log(">>> Round " + std::to_string(m_currentRound) + " Defense <<<", sf::Color(150,200,255));
    updateWindowTitle();
}

void Game::findOptimalPath() {
    m_currentPath = ::findOptimalPath(m_n, m_spawn, m_defense, m_guards);

    if (m_currentPath.empty()) {
        log("ERROR: Cannot find path from spawn to defense!", sf::Color::Red);
    } else {
        log("Optimal path found with " + std::to_string(m_currentPath.size()) + " cells");
    }
}

void Game::destroyGuardsOnPath() {
    int destroyed = 0;
    for (const auto& cell : m_currentPath) {
        if (cell == m_spawn || cell == m_defense) continue;
        if (m_guards[cell.x][cell.y] != nullptr) {
            m_guards[cell.x][cell.y].reset();
            destroyed++;
        }
    }
    if (destroyed > 0) {
        log("Destroyed " + std::to_string(destroyed) + " guards", sf::Color::Red);
    }
}

void Game::applyGuardAttacks() {
    for (int row = 0; row < m_n; ++row) {
        for (int col = 0; col < m_n; ++col) {
            if (m_guards[row][col] == nullptr) continue;
            Guard* guard = m_guards[row][col].get();

            for (int i = static_cast<int>(m_activeEnemies.size()) - 1; i >= 0; --i) {
                if (guard->inRange({row, col}, m_activeEnemies[i].getPos())) {
                    m_activeEnemies[i].takeDamage(guard->getAttack());
                    if (!m_activeEnemies[i].isAlive()) {
                        m_gold += m_activeEnemies[i].getGold();
                        log("Enemy killed! +" + std::to_string(m_activeEnemies[i].getGold()) + " gold", sf::Color::Yellow);
                        m_activeEnemies.erase(m_activeEnemies.begin() + i);
                        updateWindowTitle();
                    }
                }
            }
        }
    }
}

void Game::checkGameOver() {
    if (m_defenseHp <= 0) {
        m_gameOver = true;
        m_gameWon = false;
        m_window.setTitle("Game Over - Cube Attack");
        log("GAME OVER! Defense point destroyed!", sf::Color::Red);
        m_phase = Phase::Defense;
        showGameOverWindow();
    }
}

// 游戏结束后显示胜利或失败窗口
void Game::showGameOverWindow() {
    sf::Sound sfx(m_gameWon ? m_winSoundBuffer : m_loseSoundBuffer);
    sfx.play();

    const sf::Texture* tex = m_gameWon ? &m_winTexture : &m_loseTexture;
    int winW = 400, winH = 300;
    sf::Texture _spt;
    sf::Sprite sp(_spt);

    if (tex && tex->getSize().x > 0 && tex->getSize().y > 0) {
		sp.setTexture(*tex, true);  // 调整贴图大小
        float sc = std::min(600.0f / tex->getSize().x, 500.0f / tex->getSize().y);
        if (sc < 1.0f) sp.setScale(sf::Vector2f(sc, sc));
        auto b = sp.getLocalBounds();
        winW = std::max(400, (int)(b.size.x * (sc < 1.0f ? sc : 1.0f)));
        winH = std::max(300, (int)(b.size.y * (sc < 1.0f ? sc : 1.0f)));
    }

    sf::RenderWindow popup(sf::VideoMode(sf::Vector2u(winW, winH)),
        m_gameWon ? "Victory!" : "Game Over");

    tgui::Gui pg(popup);
    try { tgui::Font ft("resources/wryh.ttf"); pg.setFont(ft); } catch (...) {}

    auto cb = tgui::Button::create();
    cb->setPosition(popup.getSize().x / 2.0f - 50, popup.getSize().y - 50);
    cb->setSize(100, 35);
    cb->setText("Close");
    cb->setTextSize(16);
    cb->onPress([&] { popup.close(); });
    pg.add(cb);

    while (popup.isOpen()) {
        while (const auto oe = popup.pollEvent()) {
            if (oe->is<sf::Event::Closed>()) popup.close();
            pg.handleEvent(*oe);
        }
        popup.clear(sf::Color::Black);

        if (tex && tex->getSize().x > 0) {
            sp.setPosition(sf::Vector2f(
                (popup.getSize().x - sp.getGlobalBounds().size.x) / 2.0f,
                (popup.getSize().y - sp.getGlobalBounds().size.y) / 2.0f - 20));
            popup.draw(sp);
        } else {
            sf::Text fb(m_font, m_gameWon ? "Victory!" : "Game Over", 36);
            fb.setFillColor(m_gameWon ? sf::Color::Green : sf::Color::Red);
            auto tb = fb.getLocalBounds();
            fb.setOrigin(sf::Vector2f(tb.size.x / 2.0f, tb.size.y / 2.0f));
            fb.setPosition(sf::Vector2f(popup.getSize().x / 2.0f, popup.getSize().y / 2.0f - 40));
            popup.draw(fb);
        }
        pg.draw();
        popup.display();
    }
    m_window.close();
}

// 敌人刷新逻辑
void Game::spawnEnemiesForRound() {
    m_pendingEnemies.clear();
    m_activeEnemies.clear();

    int baseCount = 10 + (m_currentRound - 1) * 2;
    bool isBossRound = (m_currentRound % 5 == 0) || (m_currentRound == m_totalRounds);
    int bossCount = isBossRound ? 1 : 0;
    int normalCount = baseCount - bossCount;
    if (normalCount < 0) normalCount = 0;

    // Boss
    if (bossCount > 0) {
        int bossHp = 100;
        bossHp = static_cast<int>(bossHp * (1 + 0.1 * (m_currentRound - 1)));
        if (bossHp < 100) bossHp = 100;
        m_pendingEnemies.emplace_back(bossHp, 50, 0, m_spawn);
    }

    // 普通敌人依照概率随机刷新
    std::vector<std::pair<int, int>> enemyWeights = { {10, 2}, {5, 5} };
    std::vector<int> enemyBaseHp = {10, 30};

    int totalWeight = 0;
    for (auto& p : enemyWeights) totalWeight += p.first;

    for (int i = 0; i < normalCount; ++i) {
        int r = rand() % totalWeight;
        int accum = 0;
        int typeIndex = 0;
        for (size_t j = 0; j < enemyWeights.size(); ++j) {
            accum += enemyWeights[j].first;
            if (r < accum) {
                typeIndex = static_cast<int>(j);
                break;
            }
        }
        int baseHp = enemyBaseHp[typeIndex];
        int gold = enemyWeights[typeIndex].second;
        int hp = static_cast<int>(baseHp * (1 + 0.1 * (m_currentRound - 1)));
        if (hp < baseHp) hp = baseHp;
        m_pendingEnemies.emplace_back(hp, gold, enemyWeights[typeIndex].first, m_spawn);
    }

    log("Round " + std::to_string(m_currentRound) + ": " + std::to_string(m_pendingEnemies.size()) + " enemies", sf::Color(255,200,100));
}

void Game::updateUpgradeButtonVisibility() {
    bool show = (m_selectedGuardPos.x >= 0 && m_guards[m_selectedGuardPos.x][m_selectedGuardPos.y] != nullptr);
    m_upgAtkBtn->setVisible(show);
    m_upgWgtBtn->setVisible(show);
}

void Game::drawAttackRange() {
    if (m_selectedGuardPos.x < 0 || m_selectedGuardPos.x >= m_n) return;
    Guard* g = m_guards[m_selectedGuardPos.x][m_selectedGuardPos.y].get();
    if (!g) return;
    sf::RectangleShape rr(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    rr.setFillColor(sf::Color(255, 255, 0, 60));
    rr.setOutlineThickness(1);
    rr.setOutlineColor(sf::Color::Yellow);
    for (int r = 0; r < m_n; ++r) for (int c = 0; c < m_n; ++c) {
        if (r == m_selectedGuardPos.x && c == m_selectedGuardPos.y) continue;
        if (g->inRange(m_selectedGuardPos, {r, c})) {
            rr.setPosition(sf::Vector2f(MARGIN + c * CELL_SIZE, MARGIN + r * CELL_SIZE));
            m_window.draw(rr);
        }
    }
    sf::RectangleShape hr(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    hr.setFillColor(sf::Color::Transparent);
    hr.setOutlineThickness(3);
    hr.setOutlineColor(sf::Color::Yellow);
    hr.setPosition(sf::Vector2f(MARGIN + m_selectedGuardPos.y * CELL_SIZE, MARGIN + m_selectedGuardPos.x * CELL_SIZE));
    m_window.draw(hr);
}

// 右侧日志和属性面板绘制
void Game::drawRightPanel() {
    int rpX = MARGIN + m_n * CELL_SIZE + RIGHT_PANEL_GAP;
    int rpY = MARGIN;
    int rpH = m_n * CELL_SIZE;
    sf::RectangleShape bg(sf::Vector2f(RIGHT_PANEL_WIDTH, rpH));
    bg.setFillColor(sf::Color(30, 30, 40, 220));
    bg.setOutlineThickness(2);
    bg.setOutlineColor(sf::Color(80, 80, 100));
    bg.setPosition(sf::Vector2f(rpX, rpY));
    m_window.draw(bg);

    int lh = std::max(80, (int)(rpH * 0.55f));
    int ih = rpH - lh - 10;
    if (ih < 155) { ih = 155; lh = rpH - ih - 10; }
    int tx = rpX + 8;

    sf::Text lt(m_font, "=== Log ===", 14);
    lt.setFillColor(sf::Color(200, 200, 255));
    lt.setPosition(sf::Vector2f(tx, rpY + 5));
    m_window.draw(lt);

    int ct = rpY + 24;
    int mv = (lh - 24) / 18;
    size_t si = 0;
    if (m_log.size() > (size_t)(mv + m_logScroll)) si = m_log.size() - mv - m_logScroll;
    int dn = 0;
    for (size_t i = si; i < m_log.size() && dn < mv; ++i, ++dn) {
        sf::Text lx(m_font, m_log[i].text, 13);
        lx.setFillColor(m_log[i].color);
        lx.setPosition(sf::Vector2f(tx, ct + dn * 18));
        m_window.draw(lx);
    }

    sf::RectangleShape sl(sf::Vector2f(RIGHT_PANEL_WIDTH - 16, 1));
    sl.setFillColor(sf::Color(100, 100, 120));
    sl.setPosition(sf::Vector2f(tx, rpY + lh + 2));
    m_window.draw(sl);

    int iy = rpY + lh + 5;
    sf::Text it(m_font, "=== Info ===", 14);
    it.setFillColor(sf::Color(200, 255, 200));
    it.setPosition(sf::Vector2f(tx, iy));
    m_window.draw(it);
    iy += 18;

    auto dl = [&](const std::string& lb, const std::string& vl, const sf::Color& vc = sf::Color::White) {
        sf::Text lbt(m_font, lb, 13);
        lbt.setFillColor(sf::Color(180, 180, 180));
        lbt.setPosition(sf::Vector2f(tx, iy));
        m_window.draw(lbt);
        sf::Text vlt(m_font, vl, 13);
        vlt.setFillColor(vc);
        vlt.setPosition(sf::Vector2f(tx + 70, iy));
        m_window.draw(vlt);
        iy += 14;
    };

    dl("Gold:", std::to_string(m_gold), sf::Color::Yellow);
    dl("HP:", std::to_string(m_defenseHp), sf::Color::Green);
    dl("Enemies:", std::to_string(m_pendingEnemies.size() + m_activeEnemies.size()) + " (A:" + std::to_string(m_activeEnemies.size()) + ")", sf::Color(255, 150, 150));
    dl("Round:", std::to_string(m_currentRound) + "/" + std::to_string(m_totalRounds));
    dl("Phase:", m_phase == Phase::Defense ? "Defense" : "Attack");
    dl("Remove:", m_removeMode ? "ON" : "OFF", m_removeMode ? sf::Color::Red : sf::Color(150, 150, 150));
    iy += 2;

    if (m_selectedGuardPos.x >= 0 && m_selectedGuardPos.x < m_n) {
        Guard* g = m_guards[m_selectedGuardPos.x][m_selectedGuardPos.y].get();
        if (g) {
            std::string ds;
            switch (g->getDirection()) {
                case Direction::Up: ds = "U"; break;
                case Direction::Down: ds = "D"; break;
                case Direction::Left: ds = "L"; break;
                case Direction::Right: ds = "R"; break;
            }
            sf::Text gl(m_font, "--- Selected Guard ---", 13);
            gl.setFillColor(sf::Color(255, 255, 150));
            gl.setPosition(sf::Vector2f(tx, iy));
            m_window.draw(gl);
            iy += 16;
            dl("Type:", g->getName(), sf::Color(150, 200, 255));
            dl("Atk:", std::to_string(g->getAttack()), sf::Color(255, 150, 100));
            dl("Wgt:", std::to_string(g->getWeight()), sf::Color(200, 200, 100));
            dl("Dir:", ds, sf::Color(150, 255, 150));
        }
    }
}

