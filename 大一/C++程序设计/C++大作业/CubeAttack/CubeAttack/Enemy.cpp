#include "Enemy.h"

Enemy::Enemy(int hp, int gold, int weight, const sf::Vector2i& pos)
    : m_hp(hp), m_gold(gold), m_weight(weight), m_pos(pos), m_pathIndex(0) {
}

void Enemy::takeDamage(int dmg) {
    m_hp -= dmg;
    if (m_hp < 0) {
        m_hp = 0;
    }
}

int Enemy::getHp() const { return m_hp; }
int Enemy::getGold() const { return m_gold; }
int Enemy::getWeight() const { return m_weight; }
const sf::Vector2i& Enemy::getPos() const { return m_pos; }
bool Enemy::isAlive() const { return m_hp > 0; }
void Enemy::setPos(const sf::Vector2i& pos) { m_pos = pos; }

int Enemy::getPathIndex() const { return m_pathIndex; }
void Enemy::setPathIndex(int idx) { m_pathIndex = idx; }
