#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <optional>

// 守卫方向
enum class Direction {
    Up,
    Down,
    Left,
    Right
};

class Guard {
public:
    Guard(int attack, int weight, int cost, Direction dir);
    virtual ~Guard() = default;

    virtual std::string getName() const = 0;
    virtual bool inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const = 0;

    int getAttack() const;
    int getWeight() const;
    int getCost() const;
    Direction getDirection() const;

    void upgradeAttack();
    void upgradeWeight();
    bool canUpgradeAttack() const;
    bool canUpgradeWeight() const;

    sf::Sprite& getSprite() { return *m_sprite; }
    const sf::Sprite& getSprite() const { return *m_sprite; }
    void setupSprite(const sf::Texture& texture, const sf::Vector2f& position) {
        m_sprite.emplace(texture);
        m_sprite->setPosition(position);
    }

protected:
    int m_attack;
    int m_weight;
    int m_cost;
    Direction m_dir;
    bool m_attackUpgraded;
    int m_initialAttack;
    std::optional<sf::Sprite> m_sprite;
};

class TankGuard : public Guard {
public:
    TankGuard(Direction dir);
    std::string getName() const override;
    bool inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const override;
};

class SwordmanGuard : public Guard {
public:
    SwordmanGuard(Direction dir);
    std::string getName() const override;
    bool inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const override;
};

class ArcherGuard : public Guard {
public:
    ArcherGuard(Direction dir);
    std::string getName() const override;
    bool inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const override;
};

class MageGuard : public Guard {
public:
    MageGuard(Direction dir);
    std::string getName() const override;
    bool inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const override;
};

class PlaceholderGuard : public Guard {
public:
    PlaceholderGuard(Direction dir);
    std::string getName() const override;
    bool inRange(const sf::Vector2i& guardPos, const sf::Vector2i& enemyPos) const override;
};
