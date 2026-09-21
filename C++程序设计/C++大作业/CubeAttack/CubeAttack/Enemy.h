#pragma once
#include <SFML/System/Vector2.hpp>

class Enemy {
public:
    Enemy(int hp, int gold, int weight, const sf::Vector2i& pos);

    int getHp() const;
    int getGold() const;
    int getWeight() const;
    const sf::Vector2i& getPos() const;
    void takeDamage(int damage);
    bool isAlive() const;
    void setPos(const sf::Vector2i& pos);

    int getPathIndex() const;
    void setPathIndex(int idx);

private:
    int m_hp;
    int m_gold;
    int m_weight;
    sf::Vector2i m_pos;
    int m_pathIndex; // 当前敌人在路径中的索引
};
