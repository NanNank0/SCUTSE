#include "Guard.h"
#include <cmath>

// 守卫基类
Guard::Guard(int attack, int weight, int cost, Direction dir)
    : m_attack(attack), m_weight(weight), m_cost(cost), m_dir(dir), m_attackUpgraded(false), m_initialAttack(attack) {
}

int Guard::getAttack() const { return m_attack; }
int Guard::getWeight() const { return m_weight; }
int Guard::getCost() const { return m_cost; }
Direction Guard::getDirection() const { return m_dir; }

void Guard::upgradeAttack() {
    int maxAttack = m_initialAttack * 2;
    if (m_attack < maxAttack) {
        m_attack += 1;
        if (m_attack > maxAttack) m_attack = maxAttack;
    }
}

void Guard::upgradeWeight() {
    m_weight += 1;
}

bool Guard::canUpgradeAttack() const {
    int maxAttack = m_initialAttack * 2;
    return m_attack < maxAttack;
}
bool Guard::canUpgradeWeight() const { return true; }

// Tank
TankGuard::TankGuard(Direction dir) : Guard(1, 10, 3, dir) {}
std::string TankGuard::getName() const { return "Tank"; }
bool TankGuard::inRange(const sf::Vector2i&, const sf::Vector2i&) const { return false; }

// Swordman
SwordmanGuard::SwordmanGuard(Direction dir) : Guard(5, 4, 3, dir) {}
std::string SwordmanGuard::getName() const { return "Swordman"; }
bool SwordmanGuard::inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const {
    switch (m_dir) {
    case Direction::Up:    return (enemyPos.x == guardPos.x - 1 && enemyPos.y == guardPos.y);
    case Direction::Down:  return (enemyPos.x == guardPos.x + 1 && enemyPos.y == guardPos.y);
    case Direction::Left:  return (enemyPos.x == guardPos.x && enemyPos.y == guardPos.y - 1);
    case Direction::Right: return (enemyPos.x == guardPos.x && enemyPos.y == guardPos.y + 1);
    }
    return false;
}

// Archer
ArcherGuard::ArcherGuard(Direction dir) : Guard(5, 2, 4, dir) {}
std::string ArcherGuard::getName() const { return "Archer"; }
bool ArcherGuard::inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const {
    int dx = enemyPos.x - guardPos.x;
    int dy = enemyPos.y - guardPos.y;
    switch (m_dir) {
    case Direction::Up:
        return (dx >= -3 && dx <= -1 && std::abs(dy) <= 1);
    case Direction::Down:
        return (dx >= 1 && dx <= 3 && std::abs(dy) <= 1);
    case Direction::Left:
        return (dy >= -3 && dy <= -1 && std::abs(dx) <= 1);
    case Direction::Right:
        return (dy >= 1 && dy <= 3 && std::abs(dx) <= 1);
    }
    return false;
}

// Mage
MageGuard::MageGuard(Direction dir) : Guard(7, 1, 5, dir) {}
std::string MageGuard::getName() const { return "Mage"; }
bool MageGuard::inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const {
    int dx = enemyPos.x - guardPos.x;
    int dy = enemyPos.y - guardPos.y;
    switch (m_dir) {
    case Direction::Up:   return (dx < 0 && dy == 0 && -dx <= 6);
    case Direction::Down: return (dx > 0 && dy == 0 && dx <= 6);
    case Direction::Left: return (dy < 0 && dx == 0 && -dy <= 6);
    case Direction::Right:return (dy > 0 && dx == 0 && dy <= 6);
    }
    return false;
}

// Placeholder 滚木类
PlaceholderGuard::PlaceholderGuard(Direction dir) : Guard(2, 2, 1, dir) {}
std::string PlaceholderGuard::getName() const { return "Placeholder"; }
bool PlaceholderGuard::inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const {
    switch (m_dir) {
    case Direction::Up:    return (enemyPos.x == guardPos.x - 1 && enemyPos.y == guardPos.y);
    case Direction::Down:  return (enemyPos.x == guardPos.x + 1 && enemyPos.y == guardPos.y);
    case Direction::Left:  return (enemyPos.x == guardPos.x && enemyPos.y == guardPos.y - 1);
    case Direction::Right: return (enemyPos.x == guardPos.x && enemyPos.y == guardPos.y + 1);
    }
    return false;
}
